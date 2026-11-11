/*
 * Left half -> right half "a key was pressed" signal, for the Bongo Cat on the right screen.
 *  - central: listens to key presses, and (only if its screen is ON, rate-limited to 10/s)
 *    invokes &tapsig on the peripheral through the existing split link.
 *  - peripheral: &tapsig raises a keycode event that the cat widget listens to.
 */

#define DT_DRV_COMPAT zmk_behavior_tap_signal

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <nzmk/tap_signal_event.h>
#if !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#include <zmk/events/split_peripheral_layer_changed.h>
#include <zmk/split/peripheral_layers.h>
#endif
#include <zmk/workqueue.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#include <zmk/split/central.h>
#endif

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

/* ---- peripheral side: the behavior itself ---- */

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    raise_nzmk_tap_signal((struct nzmk_tap_signal){.state = true});
#if !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    /* idle timer: the fork's activity listener counts this event; resend the CURRENT layer mask so
     * nothing changes, at most once per second (the right screen must not blank while the left
     * hand is typing) */
    static int64_t last_poke;
    const int64_t now = k_uptime_get();
    if (now - last_poke > 1000) {
        last_poke = now;
        uint32_t layers = 0;
        for (uint8_t l = 0; l < 8; l++) {
            if (peripheral_layer_active(l)) {
                layers |= BIT(l);
            }
        }
        raise_zmk_split_peripheral_layer_changed(
            (struct zmk_split_peripheral_layer_changed){.layers = layers});
    }
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_tap_signal_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_tap_signal_driver_api);

/* ---- central side: the sender ---- */

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

#if IS_ENABLED(CONFIG_ZMK_DISPLAY)
extern bool nzmk_display_blanked(void);
#endif

#define SEND_MIN_INTERVAL_MS 100

static int64_t last_send_ms;

static void send_cb(struct k_work *work) {
    struct zmk_behavior_binding binding = {.behavior_dev = DEVICE_DT_NAME(DT_DRV_INST(0))};
    struct zmk_behavior_binding_event event = {.layer = 0, .position = 0,
                                               .timestamp = k_uptime_get()};
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
    event.source = 0;
#endif
    zmk_split_central_invoke_behavior(0, &binding, event, true);
}

static K_WORK_DEFINE(send_work, send_cb);

static int tap_signal_listener(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev == NULL || !ev->state) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    /* modifiers (LeftCtrl..RightGUI = 0xE0..0xE7) are not "typing": no signal for them */
    if (ev->usage_page == 0x07 && ev->keycode >= 0xE0 && ev->keycode <= 0xE7) {
        return ZMK_EV_EVENT_BUBBLE;
    }

#if IS_ENABLED(CONFIG_ZMK_DISPLAY)
    if (nzmk_display_blanked()) {
        return ZMK_EV_EVENT_BUBBLE;
    }
#endif

    const int64_t now = k_uptime_get();
    if (now - last_send_ms < SEND_MIN_INTERVAL_MS) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    last_send_ms = now;

    k_work_submit(&send_work); /* system workqueue: never block the queue the RGB fork uses */
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(tap_signal, tap_signal_listener);
ZMK_SUBSCRIPTION(tap_signal, zmk_keycode_state_changed);

#endif /* CONFIG_ZMK_SPLIT_ROLE_CENTRAL */

#endif /* DT_HAS_COMPAT_STATUS_OKAY */
