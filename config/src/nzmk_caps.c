/*
 * Caps tracking: host Caps Lock (HID indicators) + our two Caps Word behaviors.
 * Used by: the left screen (CAPS / WORD bar), the LED layer colors (&ugs/&ugc) and the RIP effect.
 */

#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <zmk/events/underglow_color_changed.h>
#include <dt-bindings/zmk/hid_indicators.h>
#include <nzmk/rgb_engine.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)
#include <zmk/events/hid_indicators_changed.h>
#endif

static bool host_caps;
static bool word_active[2];
static bool remote_caps; /* peripheral: what the central told us */

static void notify(void) {
    raise_zmk_underglow_color_changed(
        (struct zmk_underglow_color_changed){.layers = UINT32_MAX, .wakeup = false});
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    nzmk_rgb_request_sync();
#endif
}

int nzmk_caps_state(void) {
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)
    if (host_caps) {
        return NZMK_CAPS_LOCK;
    }
    if (word_active[0] || word_active[1]) {
        return NZMK_CAPS_WORD;
    }
    return NZMK_CAPS_NONE;
#else
    return remote_caps ? NZMK_CAPS_LOCK : NZMK_CAPS_NONE;
#endif
}

bool nzmk_caps_active(void) { return nzmk_caps_state() != NZMK_CAPS_NONE; }

void nzmk_caps_word_set(int which, bool on) {
    if (which < 0 || which > 1 || word_active[which] == on) {
        return;
    }
    word_active[which] = on;
    notify();
}

void nzmk_caps_remote(bool on) {
    if (remote_caps == on) {
        return;
    }
    remote_caps = on;
    raise_zmk_underglow_color_changed(
        (struct zmk_underglow_color_changed){.layers = UINT32_MAX, .wakeup = false});
}

uint32_t nzmk_caps_color(uint32_t normal_hex) {
    const uint32_t r = (normal_hex >> 16) & 0xFF, g = (normal_hex >> 8) & 0xFF, b = normal_hex & 0xFF;
    const bool reddish = r >= 180 && g <= 120 && b <= 100;
    return reddish ? 0x00FFFF : 0xFF0000;
}

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)
static int caps_listener(const zmk_event_t *eh) {
    const struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    const bool caps = (ev->indicators & BIT(CAPS_LOCK)) != 0;
    if (caps != host_caps) {
        host_caps = caps;
        notify();
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(nzmk_caps, caps_listener);
ZMK_SUBSCRIPTION(nzmk_caps, zmk_hid_indicators_changed);
#endif
