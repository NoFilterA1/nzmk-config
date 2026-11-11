#pragma once

#include <zmk/event_manager.h>

/* Raised on the PERIPHERAL when the central says "a key was pressed on the other half". */
struct nzmk_tap_signal {
    bool state;
};

ZMK_EVENT_DECLARE(nzmk_tap_signal);
