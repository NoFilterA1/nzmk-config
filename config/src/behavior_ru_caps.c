/*
 * &ru_caps: Caps Word for RUSSIAN words.
 * Stock &caps_word shifts only A-Z and ends on every other key. In the Russian layout the letters
 * х ъ ж э б ю ё sit on the keys [ ] ; ' , . ` so the stock behavior stops on them (and would not
 * capitalize them). This one keeps going and shifts those keys too. Ends on space/enter/slash etc.
 */

#define DT_DRV_COMPAT zmk_behavior_ru_caps

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
    nzmk_caps_word_set(1, active);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_ru_caps_driver_api = {
    .binding_pressed = on_pressed,
    .binding_released = on_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_ru_caps_driver_api);

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)

#define KEY_A 0x04
#define KEY_Z 0x1D
#define KEY_1 0x1E
#define KEY_0 0x27
#define KEY_BACKSPACE 0x2A
#define KEY_MINUS 0x2D
#define KEY_LBKT 0x2F
#define KEY_RBKT 0x30
#define KEY_SEMI 0x33
#define KEY_SQT 0x34
#define KEY_GRAVE 0x35
#define KEY_COMMA 0x36
#define KEY_DOT 0x37
#define KEY_DELETE 0x4C

static bool is_ru_letter_key(uint32_t id) {
    switch (id) {
    case KEY_LBKT:  /* х */
    case KEY_RBKT:  /* ъ */
    case KEY_SEMI:  /* ж */
    case KEY_SQT:   /* э */
    case KEY_GRAVE: /* ё */
    case KEY_COMMA: /* б */
    case KEY_DOT:   /* ю */
        return true;
    default:
        return false;
    }
}

static int ru_caps_listener(const zmk_event_t *eh) {
    struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev == NULL || !ev->state || !active || ev->usage_page != 0x07) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    const uint32_t id = ev->keycode;
    const bool alpha = id >= KEY_A && id <= KEY_Z;
    const bool numeric = id >= KEY_1 && id <= KEY_0;

    if (alpha || is_ru_letter_key(id)) {
        ev->implicit_modifiers |= MOD_LSFT;
    } else if (!numeric && !is_mod(ev->usage_page, id) && id != KEY_MINUS &&
               id != KEY_BACKSPACE && id != KEY_DELETE) {
        active = false;
        nzmk_caps_word_set(1, false);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(ru_caps, ru_caps_listener);
ZMK_SUBSCRIPTION(ru_caps, zmk_keycode_state_changed);

#endif /* central */

#endif /* DT_HAS_COMPAT_STATUS_OKAY */
