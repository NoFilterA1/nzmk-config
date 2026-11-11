/*
 * Mouse layer latch: tap the two layer keys (MO2 + MO3) together TWICE and the mouse layer (6, a copy of
 * Adjust) stays on. The next press of MO2, MO3 or Esc releases it.
 * (Layer 4 itself is driven by the conditional-layers rule and cannot be latched, hence the copy.)
 */

#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/keymap.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT)

#define MO2_POS 37
#define MO3_POS 40
#define ESC_POS 35 /* Esc also releases the latch */
#define LOCK_LAYER 6
#define WINDOW_MS 900

static bool down2, down3, locked;
static int chords;
static int64_t first_ms;

static int latch_listener(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    const bool is_mo = ev->position == MO2_POS || ev->position == MO3_POS;

    if (ev->position == ESC_POS && ev->state && locked) {
        locked = false;
        chords = 0;
        zmk_keymap_layer_deactivate(LOCK_LAYER, false);
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (!is_mo) {
        if (ev->state) {
            chords = 0; /* any other key cancels the gesture */
        }
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (ev->state && locked) { /* the next layer key press unlocks */
        locked = false;
        chords = 0;
        zmk_keymap_layer_deactivate(LOCK_LAYER, false);
    }

    if (ev->position == MO2_POS) down2 = ev->state;
    if (ev->position == MO3_POS) down3 = ev->state;

    if (ev->state && down2 && down3 && !locked) { /* both layer keys are down: one chord */
        const int64_t now = k_uptime_get();
        if (chords == 0 || now - first_ms > WINDOW_MS) {
            chords = 1;
            first_ms = now;
        } else {
            chords = 0;
            locked = true;
            zmk_keymap_layer_activate(LOCK_LAYER, false);
        }
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(layer_latch, latch_listener);
ZMK_SUBSCRIPTION(layer_latch, zmk_position_state_changed);

#endif
