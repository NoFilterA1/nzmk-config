/*
 * RGB engine on top of the darknao per-layer RGB fork:
 *  - extra effects: ripple (RIP) and fire driven by typing speed (FIR)
 *  - battery view while a key is held
 *  - all RGB commands run on the central; it pushes the resulting state to the peripheral
 *    (so both halves always have the same color/mode)
 *  - hold-to-repeat support lives in behavior_led_rgb.c
 */

#include <math.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/settings/settings.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/events/underglow_color_changed.h>
#include <zmk/battery.h>
#include <zmk/usb.h>
#include <zmk/workqueue.h>
#include <zmk/rgb_underglow.h>
#include <zmk/rgb_underglow_layer.h>
#include <dt-bindings/zmk/rgb.h>
#include <nzmk/rgb_engine.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#include <zmk/split/central.h>
#endif

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_CHOSEN(zmk_underglow)

#define STRIP_NODE DT_CHOSEN(zmk_underglow)
#define NPIX DT_PROP(STRIP_NODE, chain_length)
#define KEYS 42
#define TICK_MS 33
#define CMD_SELECT_LAYER_EFFECT 100

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);
static struct led_rgb px[NPIX];

static int mode;
static bool mode_init;
static bool in_fx;
static bool fx_on = true;
static bool bat_view;
static bool restore_fork_on;
static int restore_effect = -1;     /* fork effect to bring back after the battery view */
static int64_t bat_deadline;        /* peripheral watchdog: view ends if no refresh arrives */
#define BAT_WATCHDOG_MS 700
#define BAT_REFRESH_MS 300
static int8_t led_of_pos[KEYS];

/* ---------------- helpers ---------------- */

static void ensure_init(void) {
    if (mode_init) {
        return;
    }
    memset(led_of_pos, -1, sizeof(led_of_pos));
    for (int i = 0; i < NPIX; i++) {
        const int p = rgb_pixel_lookup(i);
        if (p >= 0 && p < KEYS) {
            led_of_pos[p] = i;
        }
    }
    mode = zmk_rgb_underglow_calc_effect(0);
    mode_init = true;
}

static void redraw_layers(bool wakeup) {
    raise_zmk_underglow_color_changed(
        (struct zmk_underglow_color_changed){.layers = UINT32_MAX, .wakeup = wakeup});
}

static struct led_rgb hsv(float h, float s, float v) {
    if (v < 0) v = 0;
    if (v > 1) v = 1;
    const int i = ((int)(h / 60.0f)) % 6;
    const float f = h / 60.0f - (int)(h / 60.0f);
    const float p = v * (1 - s), q = v * (1 - f * s), t = v * (1 - (1 - f) * s);
    float r, g, b;
    switch (i) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    default: r = v; g = p; b = q; break;
    }
    return (struct led_rgb){.r = (uint8_t)(r * 255.0f), .g = (uint8_t)(g * 255.0f),
                            .b = (uint8_t)(b * 255.0f)};
}

static uint32_t rng_state = 2463534242u;
static uint32_t rng_next(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static float frand(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return (rng_state & 0xFFFF) / 65535.0f;
}

static void key_xy(int p, float *x, float *y) {
    if (p < 36) {
        *x = p % 12;
        *y = p / 12;
    } else {
        *x = (p - 36) + 3;
        *y = 3;
    }
}

/* ---------------- sync (central -> peripheral) ---------------- */

static uint32_t pack_state(void) {
    const struct zmk_led_hsb c = zmk_rgb_underglow_calc_brt(0);
    return (nzmk_rgb_is_on() ? 1u : 0u) | ((uint32_t)(mode & 7) << 1) |
           ((uint32_t)(c.h & 0x1FF) << 4) | ((uint32_t)(c.s & 0x7F) << 13) |
           ((uint32_t)(c.b & 0x7F) << 20) | ((nzmk_caps_active() ? 1u : 0u) << 27);
}

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void sync_cb(struct k_work *work) {
    struct zmk_behavior_binding binding = {.behavior_dev = "lsync", .param1 = pack_state()};
    struct zmk_behavior_binding_event event = {.layer = 0, .position = 0,
                                               .timestamp = k_uptime_get()};
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
    event.source = 0;
#endif
    zmk_split_central_invoke_behavior(0, &binding, event, true);
}
static K_WORK_DELAYABLE_DEFINE(sync_work, sync_cb);

static void schedule_sync(int delay_ms) {
    if (delay_ms > 0) {
        k_work_reschedule(&sync_work, K_MSEC(delay_ms));
    } else {
        k_work_schedule(&sync_work, K_MSEC(60));
    }
}
#else
static void schedule_sync(int delay_ms) {}
#endif

void nzmk_rgb_request_sync(void) {
    schedule_sync(0);
}

/* ---------------- persistence of the fx mode ---------------- */

static uint8_t saved_mode = 255;

static void save_mode(void) {
    const uint8_t v = in_fx ? (uint8_t)mode : 255;
    settings_save_one("nzmk/fx_mode", &v, 1);
}

/* ---------------- effects ---------------- */

struct ripple {
    int64_t t0;
    float x, y;
    float amp;  /* 1.0 = normal key press; stronger when the keyboard is "charged" */
    bool active;
};
static struct ripple ripples[6];
static bool held[KEYS];            /* keys currently held (the ripple keeps pulsing) */
static int64_t held_emit[KEYS];
#define HOLD_PULSE_MS 420

/* INV secret ("overload"): every tap of this half's layer key charges the keyboard; the 10th tap breaks
 * through and the keyboard keeps emitting waves by itself; 10 more taps calm it down. */
static float charge;          /* 0..10, grows with every tap, leaks away when you stop tapping */
static int tap_count;
static int64_t last_tap_ms;
static bool overload;
static int64_t next_auto_ms;
static float burst_flash;     /* white flash of the breakthrough, fades out */
#define TAPS_FOR_OVERLOAD 10
#define TAP_GAP_MS 600

static uint32_t rng_next(void);

static void add_ripple_amp(int pos, float amp) {
    float x, y;
    key_xy(pos, &x, &y);
    int slot = 0;
    int64_t oldest = INT64_MAX;
    for (int i = 0; i < 6; i++) {
        if (!ripples[i].active) {
            slot = i;
            oldest = INT64_MIN;
            break;
        }
        if (ripples[i].t0 < oldest) {
            oldest = ripples[i].t0;
            slot = i;
        }
    }
    ripples[slot] = (struct ripple){.t0 = k_uptime_get(), .x = x, .y = y, .amp = amp, .active = true};
}

static void add_ripple(int pos) { add_ripple_amp(pos, 1.0f); }

/* the breakthrough: three big waves from the pressed key (staggered via the amp/age trick: spawn now) */
static void burst(int pos) {
    add_ripple_amp(pos, 3.0f);
    add_ripple_amp(pos, 2.2f);
    add_ripple_amp(pos, 1.6f);
    burst_flash = 1.0f;
}

static int random_key_pos(void) {
    int n = 0, pos = 0;
    for (int k = 0; k < KEYS; k++) {
        if (led_of_pos[k] >= 0) {
            n++;
        }
    }
    if (n == 0) {
        return 0;
    }
    int pick = rng_next() % n;
    for (int k = 0; k < KEYS; k++) {
        if (led_of_pos[k] >= 0 && pick-- == 0) {
            pos = k;
            break;
        }
    }
    return pos;
}

static void render_ripple(int64_t now) {
    const struct zmk_led_hsb c = zmk_rgb_underglow_calc_brt(0);
    const float bright = c.b / 100.0f;
    const bool inv = (mode == NZMK_RGB_MODE_INV);
    const bool caps = !inv && nzmk_caps_active();
    /* red unless the current solid color is already reddish, then cyan (RIP + Caps) */
    const float caps_h = (c.s > 30 && (c.h < 20 || c.h > 340)) ? 180.0f : 0.0f;
    const float hue = c.h;
    const float inv_h = fmodf(hue + 180.0f, 360.0f); /* the inverted (complementary) color */
    const float base_h = inv ? inv_h : caps_h;
    const bool has_base = inv || caps;

    /* --- overload / charge dynamics (INV only) --- */
    if (inv) {
        charge *= 0.985f;
        burst_flash *= 0.90f;
        if (overload && now >= next_auto_ms) {
            add_ripple_amp(random_key_pos(), 1.8f);
            next_auto_ms = now + 220 + (rng_next() % 480);
            if ((rng_next() & 7) == 0) { /* now and then a double surge */
                add_ripple_amp(random_key_pos(), 2.4f);
            }
        }
    }

    memset(px, 0, sizeof(px));

    /* a held key keeps sending new waves */
    for (int k = 0; k < KEYS; k++) {
        if (held[k] && now - held_emit[k] >= HOLD_PULSE_MS) {
            held_emit[k] = now;
            add_ripple_amp(k, 1.0f + (inv ? charge * 0.18f : 0.0f));
        }
    }

    /* energy of the waves that are alive, used for the "floor" LEDs and the whole-board pulse */
    float pulse = 0;
    for (int r = 0; r < 6; r++) {
        if (!ripples[r].active) {
            continue;
        }
        const float age = (float)(now - ripples[r].t0);
        if (age > 1400.0f) {
            ripples[r].active = false;
            continue;
        }
        pulse += ripples[r].amp * expf(-age / 450.0f);
    }
    if (pulse > 1.0f) pulse = 1.0f;

    const float pressure = inv ? charge * 0.035f : 0.0f; /* the keys glow stronger as it charges */

    for (int i = 0; i < NPIX; i++) {
        const int p = rgb_pixel_lookup(i);

        if (p < 0 || p >= KEYS) {
            /* "floor" LEDs: dim base + pulse of the wave color */
            float v = (0.10f + 0.6f * pulse + pressure) * bright;
            px[i] = hsv(hue, c.s / 100.0f, v);
            if (has_base) {
                struct led_rgb base = hsv(base_h, 1.0f, (0.30f + 0.3f * pressure) * bright);
                if (base.r > px[i].r) px[i].r = base.r;
                if (base.g > px[i].g) px[i].g = base.g;
                if (base.b > px[i].b) px[i].b = base.b;
            }
            if (burst_flash > 0.02f) {
                const uint8_t f = (uint8_t)(burst_flash * 200.0f * bright);
                if (f > px[i].r) px[i].r = f;
                if (f > px[i].g) px[i].g = f;
                if (f > px[i].b) px[i].b = f;
            }
            continue;
        }

        float x, y;
        key_xy(p, &x, &y);
        float amp = 0;
        for (int r = 0; r < 6; r++) {
            if (!ripples[r].active) {
                continue;
            }
            const float age = (float)(now - ripples[r].t0);
            const float dx = x - ripples[r].x, dy = y - ripples[r].y;
            const float d = sqrtf(dx * dx + dy * dy);
            const float radius = age * 0.008f; /* 8 keys per second */
            const float width = 1.1f + 0.25f * (ripples[r].amp - 1.0f);
            const float w = width - fabsf(d - radius);
            if (w > 0) {
                amp += w * ripples[r].amp * expf(-age / 600.0f);
            }
        }
        /* sparks: the more charged, the more keys flicker at random */
        if (inv && charge > 1.5f && (rng_next() % 1000) < (uint32_t)(charge * 6.0f)) {
            amp += 0.7f;
        }
        px[i] = hsv(hue, c.s / 100.0f, (amp + pressure * 0.4f) * bright);
        if (has_base) {
            struct led_rgb base = hsv(base_h, 1.0f, (0.35f + pressure) * bright);
            if (base.r > px[i].r) px[i].r = base.r;
            if (base.g > px[i].g) px[i].g = base.g;
            if (base.b > px[i].b) px[i].b = base.b;
        }
        if (burst_flash > 0.02f) { /* the breakthrough flash */
            const uint8_t f = (uint8_t)(burst_flash * 220.0f * bright);
            if (f > px[i].r) px[i].r = f;
            if (f > px[i].g) px[i].g = f;
            if (f > px[i].b) px[i].b = f;
        }
    }
}

static void render_battery(void) {
    int level = zmk_battery_state_of_charge();
    if (level > 100) level = 100;
    int keys = 0;
    for (int i = 0; i < NPIX; i++) {
        const int p = rgb_pixel_lookup(i);
        if (p >= 0 && p < KEYS) keys++;
    }
    const int green = (level * keys + 50) / 100;
    int n = 0;
    for (int i = 0; i < NPIX; i++) {
        const int p = rgb_pixel_lookup(i);
        if (p < 0 || p >= KEYS) {
            px[i] = zmk_usb_is_powered() ? (struct led_rgb){.r = 0, .g = 0, .b = 60}
                                         : (struct led_rgb){.r = 0, .g = 0, .b = 0};
            continue;
        }
        px[i] = (n < green) ? (struct led_rgb){.r = 0, .g = 80, .b = 0}
                            : (struct led_rgb){.r = 70, .g = 0, .b = 0};
        n++;
    }
}

static void battery_end(void);

static void tick_cb(struct k_work *work) {
    if (bat_view) {
#if !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
        if (k_uptime_get() > bat_deadline) {
            LOG_WRN("NZMK battery watchdog: release lost, ending view");
            battery_end();
            return;
        }
#endif
        render_battery();
    } else if (in_fx && fx_on) {
        render_ripple(k_uptime_get());
    } else if (in_fx) {
        memset(px, 0, sizeof(px));
        led_strip_update_rgb(strip, px, NPIX);
        return; /* RGB switched off in an fx mode: stop ticking */
    } else {
        return;
    }
    led_strip_update_rgb(strip, px, NPIX);
    k_work_reschedule(k_work_delayable_from_work(work), K_MSEC(TICK_MS));
}
static K_WORK_DELAYABLE_DEFINE(tick_work, tick_cb);

/* ---------------- mode handling ---------------- */

static void restart_animation_if_needed(void) {
    if (mode == NZMK_RGB_MODE_LAYER) {
        return;
    }
    bool on = false;
    zmk_rgb_underglow_get_state(&on);
    if (on) {
        zmk_rgb_underglow_on();
    }
}

static void enter_fx(int m) {
    if (!in_fx) {
        bool on = false;
        zmk_rgb_underglow_get_state(&on);
        fx_on = on;
        zmk_rgb_underglow_transient_off();
        /* The fork stays in LAY otherwise and repaints the layer colors on every layer change
         * (that was the short "layer color" flash). A non-layer effect makes it ignore layers. */
        zmk_rgb_underglow_select_effect(0);
        in_fx = true;
    }
    mode = m;
    memset(ripples, 0, sizeof(ripples));
    memset(held, 0, sizeof(held));
    overload = false;
    charge = 0;
    tap_count = 0;
    burst_flash = 0;
    save_mode();
    k_work_reschedule(&tick_work, K_MSEC(60)); /* let the fork finish writing its "off" frame */
}

static void leave_fx(int m) {
    in_fx = false;
    mode = m;
    if (!bat_view) {
        k_work_cancel_delayable(&tick_work);
    }
    zmk_rgb_underglow_select_effect(m);
    if (fx_on) {
        zmk_rgb_underglow_on();
        redraw_layers(true);
    }
    save_mode();
}

static void set_mode(int m) {
    if (m >= NZMK_RGB_MODE_RIPPLE) {
        enter_fx(m);
    } else if (in_fx) {
        leave_fx(m);
    } else {
        mode = m;
        zmk_rgb_underglow_select_effect(m);
        restart_animation_if_needed();
    }
}

/* ---------------- public API ---------------- */

int nzmk_rgb_mode(void) {
    ensure_init();
    return mode;
}

bool nzmk_rgb_is_on(void) {
    ensure_init();
    if (in_fx) {
        return fx_on;
    }
    bool on = false;
    zmk_rgb_underglow_get_state(&on);
    return on;
}

bool nzmk_rgb_fx_active(void) { return in_fx || bat_view; }

void nzmk_rgb_command(uint32_t cmd) {
    ensure_init();
    LOG_WRN("NZMK cmd %u mode=%d in_fx=%d", (unsigned)cmd, mode, in_fx);
    switch (cmd) {
    case RGB_TOG_CMD:
        if (in_fx) {
            fx_on = !fx_on;
            k_work_reschedule(&tick_work, K_MSEC(5));
        } else {
            zmk_rgb_underglow_toggle();
        }
        break;
    case RGB_HUI_CMD: zmk_rgb_underglow_change_hue(1); break;
    case RGB_HUD_CMD: zmk_rgb_underglow_change_hue(-1); break;
    case RGB_SAI_CMD: zmk_rgb_underglow_change_sat(1); break;
    case RGB_SAD_CMD: zmk_rgb_underglow_change_sat(-1); break;
    case RGB_BRI_CMD: zmk_rgb_underglow_change_brt(1); break;
    case RGB_BRD_CMD: zmk_rgb_underglow_change_brt(-1); break;
    case RGB_SPI_CMD: zmk_rgb_underglow_change_spd(1); break;
    case RGB_SPD_CMD: zmk_rgb_underglow_change_spd(-1); break;
    case RGB_EFF_CMD: set_mode((mode + 1) % NZMK_RGB_MODE_COUNT); break;
    case RGB_EFR_CMD: set_mode((mode + NZMK_RGB_MODE_COUNT - 1) % NZMK_RGB_MODE_COUNT); break;
    case CMD_SELECT_LAYER_EFFECT: set_mode(NZMK_RGB_MODE_LAYER); break;
    default: break;
    }
    redraw_layers(false);
    schedule_sync(0);
}

void nzmk_rgb_apply_sync(uint32_t packed) {
    ensure_init();
    LOG_WRN("NZMK sync packed=%08x", (unsigned)packed);
    const bool on = packed & 1u;
    const int m = (packed >> 1) & 7u;
    const struct zmk_led_hsb c = {.h = (packed >> 4) & 0x1FF, .s = (packed >> 13) & 0x7F,
                                  .b = (packed >> 20) & 0x7F};
    if (m >= NZMK_RGB_MODE_COUNT) {
        return;
    }
    zmk_rgb_underglow_set_hsb(c);
    nzmk_caps_remote((packed >> 27) & 1u);

    if (m >= NZMK_RGB_MODE_RIPPLE) {
        if (!in_fx || mode != m) {
            enter_fx(m);
        }
        fx_on = on;
        k_work_reschedule(&tick_work, K_MSEC(5));
    } else {
        fx_on = on;
        if (in_fx) {
            leave_fx(m);
        } else if (mode != m) {
            mode = m;
            zmk_rgb_underglow_select_effect(m);
        }
        bool cur = false;
        zmk_rgb_underglow_get_state(&cur);
        if (on && !cur) {
            zmk_rgb_underglow_on();
        } else if (!on && cur) {
            zmk_rgb_underglow_off();
        }
        if (on) {
            restart_animation_if_needed();
        }
    }
    redraw_layers(false);
}

static void restore_cb(struct k_work *work) {
    if (!bat_view && !in_fx && restore_fork_on) {
        zmk_rgb_underglow_transient_on();
        redraw_layers(true);
    }
}
static K_WORK_DELAYABLE_DEFINE(restore_work, restore_cb);

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
/* while the key is held the central keeps telling the right half "still held" */
static void bat_refresh_cb(struct k_work *work) {
    if (!bat_view) {
        return;
    }
    struct zmk_behavior_binding binding = {.behavior_dev = "lbat"};
    struct zmk_behavior_binding_event event = {.layer = 0, .position = 0,
                                               .timestamp = k_uptime_get()};
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
    event.source = 0;
#endif
    zmk_split_central_invoke_behavior(0, &binding, event, true);
    k_work_reschedule(k_work_delayable_from_work(work), K_MSEC(BAT_REFRESH_MS));
}
static K_WORK_DELAYABLE_DEFINE(bat_refresh_work, bat_refresh_cb);
#endif

static void battery_end(void) {
    bat_view = false;
    if (!in_fx) {
        k_work_cancel_delayable(&tick_work);
        if (restore_effect >= 0) {
            zmk_rgb_underglow_select_effect(restore_effect);
            restore_effect = -1;
        }
        if (restore_fork_on) {
            zmk_rgb_underglow_transient_on();
            redraw_layers(true);
            k_work_reschedule(&restore_work, K_MSEC(100)); /* second pass: beats a late "off" frame */
        }
    }
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    k_work_cancel_delayable(&bat_refresh_work);
#endif
}

void nzmk_rgb_battery_view(bool on) {
    ensure_init();
    LOG_WRN("NZMK battery_view(%d) bat_view=%d in_fx=%d restore_on=%d", on, bat_view, in_fx,
            restore_fork_on);
    if (on) {
        bat_deadline = k_uptime_get() + BAT_WATCHDOG_MS;
        if (!bat_view) {
            if (!in_fx) {
                bool was_on = false;
                zmk_rgb_underglow_get_state(&was_on);
                restore_fork_on = was_on;
                zmk_rgb_underglow_transient_off();
                restore_effect = zmk_rgb_underglow_calc_effect(0);
                if (restore_effect == NZMK_RGB_MODE_LAYER) {
                    zmk_rgb_underglow_select_effect(0); /* layer changes must not repaint the view */
                } else {
                    restore_effect = -1;
                }
            }
            bat_view = true;
            k_work_reschedule(&tick_work, K_MSEC(60));
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
            k_work_reschedule(&bat_refresh_work, K_MSEC(BAT_REFRESH_MS));
#endif
        }
    } else if (bat_view) {
        battery_end();
    }
}

/* ---------------- events ---------------- */

static int nzmk_rgb_listener(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *pe = as_zmk_position_state_changed(eh);
    if (pe != NULL) {
        if (!in_fx || pe->position >= KEYS || led_of_pos[pe->position] < 0) {
            return ZMK_EV_EVENT_BUBBLE;
        }
        if (mode >= NZMK_RGB_MODE_RIPPLE && fx_on) {
            if (pe->state) {
                held[pe->position] = true;
                held_emit[pe->position] = k_uptime_get();
                add_ripple(pe->position);
            } else {
                held[pe->position] = false;
            }
        }
        /* INV secret: 10 quick taps of this half's layer key (left: MO2 pos 37, right: MO3 pos 40) */
        if (mode == NZMK_RGB_MODE_INV && pe->state && (pe->position == 37 || pe->position == 40)) {
            const int64_t now = k_uptime_get();
            if (now - last_tap_ms > TAP_GAP_MS) {
                tap_count = 0;
            }
            last_tap_ms = now;
            tap_count++;
            charge += 1.0f;
            if (charge > 10.0f) charge = 10.0f;
            if (tap_count >= TAPS_FOR_OVERLOAD) {
                tap_count = 0;
                overload = !overload;
                if (overload) {
                    burst(pe->position);
                    next_auto_ms = now + 600;
                } else {
                    charge = 0;
                    burst_flash = 0.4f;
                }
            } else {
                /* each hit is a little stronger than the last */
                add_ripple_amp(pe->position, 1.0f + charge * 0.18f);
            }
        }
        return ZMK_EV_EVENT_BUBBLE;
    }

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    const struct zmk_split_peripheral_status_changed *se = as_zmk_split_peripheral_status_changed(eh);
    if (se != NULL && se->connected) {
        schedule_sync(800); /* a freshly connected right half gets the current state */
    }
#endif
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(nzmk_rgb, nzmk_rgb_listener);
ZMK_SUBSCRIPTION(nzmk_rgb, zmk_position_state_changed);
ZMK_SUBSCRIPTION(nzmk_rgb, zmk_split_peripheral_status_changed);

/* ---------------- boot: restore a saved fx mode ---------------- */

static void boot_cb(struct k_work *work) {
    ensure_init();
    if (saved_mode >= NZMK_RGB_MODE_RIPPLE && saved_mode < NZMK_RGB_MODE_COUNT) {
        enter_fx(saved_mode);
    }
}
static K_WORK_DELAYABLE_DEFINE(boot_work, boot_cb);

static int nzmk_settings_set(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg) {
    const char *next;
    if (settings_name_steq(name, "fx_mode", &next) && !next) {
        if (len != 1) {
            return -EINVAL;
        }
        read_cb(cb_arg, &saved_mode, 1);
        return 0;
    }
    return -ENOENT;
}

static int nzmk_settings_commit(void) {
    if (saved_mode >= NZMK_RGB_MODE_RIPPLE && saved_mode < NZMK_RGB_MODE_COUNT) {
        k_work_schedule(&boot_work, K_MSEC(1000));
    }
    return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(nzmk_rgb, "nzmk", NULL, nzmk_settings_set, nzmk_settings_commit,
                               NULL);

#else
int nzmk_rgb_mode(void) { return 0; }
bool nzmk_rgb_is_on(void) { return false; }
bool nzmk_rgb_fx_active(void) { return false; }
void nzmk_rgb_command(uint32_t cmd) {}
void nzmk_rgb_apply_sync(uint32_t packed) {}
void nzmk_rgb_battery_view(bool on) {}
#endif
