/*
 * Language switch gesture: tap Ctrl+Shift (both together, one finger) TWICE quickly, with no other
 * key in between -> sends Win+K (the niri "switch-layout next" bind). Normal Ctrl+Shift shortcuts are
 * not affected: nothing is swallowed, the second chord within the window is what triggers it.
 */

#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <zmk/keys.h>
#include <dt-bindings/zmk/keys.h>
#include <dt-bindings/zmk/modifiers.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)

#define GESTURE_WINDOW_MS 700

static bool ctrl_down, shift_down;
static int chords;
static int64_t first_chord_ms;

static void release_cb(struct k_work *work) {
    raise_zmk_keycode_state_changed_from_encoded(LG(K), false, k_uptime_get());
    zmk_hid_masked_modifiers_clear();
}
static K_WORK_DELAYABLE_DEFINE(release_work, release_cb);

static int gesture_listener(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev == NULL || ev->usage_page != 0x07) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    const uint32_t id = ev->keycode;
    const bool is_ctrl = id == 0xE0 || id == 0xE4;
    const bool is_shift = id == 0xE1 || id == 0xE5;

    if (is_ctrl) ctrl_down = ev->state;
    if (is_shift) shift_down = ev->state;

    if (!(is_ctrl || is_shift)) {
        if (ev->state && !is_mod(ev->usage_page, id)) {
            chords = 0; /* any other key cancels the gesture */
        }
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (ev->state && ctrl_down && shift_down) { /* the chord is completed */
        const int64_t now = k_uptime_get();
        if (chords == 0 || now - first_chord_ms > GESTURE_WINDOW_MS) {
            chords = 1;
            first_chord_ms = now;
        } else {
            chords = 0;
            /* Ctrl+Shift are still held: hide them, the OS must see a clean Win+K */
            zmk_hid_masked_modifiers_set(MOD_LCTL | MOD_RCTL | MOD_LSFT | MOD_RSFT);
            raise_zmk_keycode_state_changed_from_encoded(LG(K), true, now);
            k_work_reschedule(&release_work, K_MSEC(15));
        }
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(lang_gesture, gesture_listener);
ZMK_SUBSCRIPTION(lang_gesture, zmk_keycode_state_changed);

#endif
