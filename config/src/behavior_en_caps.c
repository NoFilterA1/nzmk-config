/*
 * &en_caps: same rules as the stock &caps_word (A-Z shifted; digits, - _ Backspace Delete continue;
 * anything else ends it) but it reports its state (screen CAPS/WORD bar, red key LEDs).
 */

#define DT_DRV_COMPAT zmk_behavior_en_caps

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/keys.h>
#include <dt-bindings/zmk/modifiers.h>
#include <nzmk/rgb_engine.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#endif

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static bool active;

static int on_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    active = !active;
    nzmk_caps_word_set(0, active);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_en_caps_driver_api = {
    .binding_pressed = on_pressed,
    .binding_released = on_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_en_caps_driver_api);

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)

static int en_caps_listener(const zmk_event_t *eh) {
    struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev == NULL || !ev->state || !active || ev->usage_page != 0x07) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    const uint32_t id = ev->keycode;
    const bool alpha = id >= 0x04 && id <= 0x1D;
    const bool numeric = id >= 0x1E && id <= 0x27;

    if (alpha) {
        ev->implicit_modifiers |= MOD_LSFT;
    } else if (!numeric && !is_mod(ev->usage_page, id) && id != 0x2D /* - _ */ &&
               id != 0x2A /* Backspace */ && id != 0x4C /* Delete */) {
        active = false;
        nzmk_caps_word_set(0, false);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(en_caps, en_caps_listener);
ZMK_SUBSCRIPTION(en_caps, zmk_keycode_state_changed);

#endif

#endif
