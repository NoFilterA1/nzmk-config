/*
 * &snip <key> <id>: a letter key that types a text snippet while the "snippet chord" is active.
 *   &snip 0 0       - the chord key itself (put it on Ctrl+Shift+Backspace): press = snippet mode on
 *   &snip G 1       - normal letter G; with the chord held it types snippet #1 instead
 * The physical Ctrl/Shift stay held, so they are masked while typing.
 * Text is typed with US-layout key codes; segments for the Russian layout are separated by \x01
 * (the macro presses the OS layout switch Win+K; it assumes it starts in the ENGLISH layout and
 * switches back to English at the end).
 */

#define DT_DRV_COMPAT zmk_behavior_snippet

#include <string.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zmk/keys.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <dt-bindings/zmk/keys.h>
#include <dt-bindings/zmk/modifiers.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) && (IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) || !IS_ENABLED(CONFIG_ZMK_SPLIT))

#define SW "\x01"

/* index = snippet id (1-based) */
static const char *const snippets[] = {
    NULL,
    "goal",    /* 1  g */
    "style",   /* 2  s */
    "rule",    /* 3  r */
    "context", /* 4  c */
    "theme",   /* 5  t */
    "format",  /* 6  f */
    /* 7  p */
    "style=swear,expression,emoji,withoutwater,cynic," SW "на одну категорию" SW "/" SW "тему" SW
    "=short(~70 " SW "слов примерный потолок, желательно меньше, если не получается и теряется суть "
    "то превышай" SW,
};
#define SNIPPET_COUNT (sizeof(snippets) / sizeof(snippets[0]))

#define KEY_STEP_MS 10
#define LANG_WAIT_MS 150

static bool chord_mode;
static bool pass_through[SNIPPET_COUNT];

/* ---- character -> key code ---- */

/* Russian (ЙЦУКЕН) letters on US key positions */
static uint32_t ru_letter(uint32_t cp) {
    static const struct { uint32_t cp; uint32_t key; } map[] = {
        {0x439, Q}, {0x446, W}, {0x443, E}, {0x43A, R}, {0x435, T}, {0x43D, Y}, {0x433, U},
        {0x448, I}, {0x449, O}, {0x437, P}, {0x445, LBKT}, {0x44A, RBKT}, {0x444, A}, {0x44B, S},
        {0x432, D}, {0x430, F}, {0x43F, G}, {0x440, H}, {0x43E, J}, {0x43B, K}, {0x434, L},
        {0x436, SEMI}, {0x44D, SQT}, {0x44F, Z}, {0x447, X}, {0x441, C}, {0x43C, V}, {0x438, B},
        {0x442, N}, {0x44C, M}, {0x431, COMMA}, {0x44E, DOT}, {0x451, GRAVE},
    };
    for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        if (map[i].cp == cp) {
            return map[i].key;
        }
    }
    return 0;
}

static const uint32_t letters[26] = {A, B, C, D, E, F, G, H, I, J, K, L, M,
                                     N, O, P, Q, R, S, T, U, V, W, X, Y, Z};
static const uint32_t digits[10] = {N0, N1, N2, N3, N4, N5, N6, N7, N8, N9};

static uint32_t char_to_key(uint32_t cp, bool russian) {
    if (cp == ' ') return SPACE;
    if (russian) {
        if (cp == ',') return LS(FSLH); /* RU layout: comma = Shift + / */
        if (cp == '.') return FSLH;     /* RU layout: period = / */
        return ru_letter(cp);
    }
    if (cp >= 'a' && cp <= 'z') return letters[cp - 'a'];
    if (cp >= 'A' && cp <= 'Z') return LS(letters[cp - 'A']);
    if (cp >= '0' && cp <= '9') return digits[cp - '0'];
    switch (cp) {
    case '=': return EQUAL;
    case ',': return COMMA;
    case '.': return DOT;
    case '/': return FSLH;
    case '-': return MINUS;
    case '(': return LS(N9);
    case ')': return LS(N0);
    case '~': return LS(GRAVE);
    default: return 0;
    }
}

/* ---- typing engine (one key per work item) ---- */

static const char *cursor;
static bool russian;
static bool pressed_phase;
static uint32_t current_key;
static bool busy;

static bool sending; /* true while WE raise events (so we do not abort ourselves) */

static void send(uint32_t encoded, bool pressed) {
    sending = true;
    raise_zmk_keycode_state_changed_from_encoded(encoded, pressed, k_uptime_get());
    sending = false;
}

static void type_cb(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(type_work, type_cb);

static void finish(void) {
    zmk_hid_masked_modifiers_clear();
    busy = false;
}

static void type_cb(struct k_work *work) {
    /* Backspace release etc. clear the mask: set it again before every key */
    zmk_hid_masked_modifiers_set(MOD_LCTL | MOD_RCTL | MOD_LSFT | MOD_RSFT);
    if (pressed_phase) { /* release the key pressed last time */
        send(current_key, false);
        pressed_phase = false;
        k_work_reschedule(&type_work, K_MSEC(KEY_STEP_MS));
        return;
    }

    if (cursor == NULL || *cursor == '\0') {
        finish();
        return;
    }

    const unsigned char c = (unsigned char)*cursor;
    if (c == 0x01) { /* switch the OS layout */
        cursor++;
        russian = !russian;
        current_key = LG(K);
        send(current_key, true);
        pressed_phase = true;
        k_work_reschedule(&type_work, K_MSEC(LANG_WAIT_MS));
        return;
    }

    uint32_t cp = c;
    if ((c & 0xE0) == 0xC0 && cursor[1] != '\0') { /* 2-byte UTF-8 (Cyrillic) */
        cp = ((c & 0x1F) << 6) | ((unsigned char)cursor[1] & 0x3F);
        cursor += 2;
    } else {
        cursor++;
    }

    const uint32_t key = char_to_key(cp, russian);
    if (key == 0) {
        k_work_reschedule(&type_work, K_MSEC(1));
        return;
    }
    current_key = key;
    send(key, true);
    pressed_phase = true;
    k_work_reschedule(&type_work, K_MSEC(KEY_STEP_MS));
}

static void start_typing(uint32_t id) {
    if (busy || id == 0 || id >= SNIPPET_COUNT) {
        return;
    }
    busy = true;
    cursor = snippets[id];
    russian = false;
    pressed_phase = false;
    zmk_hid_masked_modifiers_set(MOD_LCTL | MOD_RCTL | MOD_LSFT | MOD_RSFT);
    k_work_reschedule(&type_work, K_MSEC(KEY_STEP_MS));
}

/* any real key press stops the typing immediately */
static void abort_typing(void) {
    k_work_cancel_delayable(&type_work);
    if (pressed_phase) {
        send(current_key, false);
        pressed_phase = false;
    }
    cursor = NULL;
    finish();
}

static int abort_listener(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev != NULL && ev->state && busy && !sending) {
        abort_typing();
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(snip_abort, abort_listener);
ZMK_SUBSCRIPTION(snip_abort, zmk_keycode_state_changed);

/* ---- behavior ---- */

static int on_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    const uint32_t key = binding->param1;
    const uint32_t id = binding->param2;

    if (id == 0) { /* the chord key (Backspace with Ctrl+Shift) */
        chord_mode = true;
        return ZMK_BEHAVIOR_OPAQUE;
    }

    if (id < SNIPPET_COUNT) {
        if (chord_mode) {
            pass_through[id] = false;
            start_typing(id);
            return ZMK_BEHAVIOR_OPAQUE;
        }
        pass_through[id] = true;
    }
    send(key, true); /* normal letter */
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    const uint32_t id = binding->param2;

    if (id == 0) {
        chord_mode = false;
        return ZMK_BEHAVIOR_OPAQUE;
    }
    if (id < SNIPPET_COUNT && !pass_through[id]) {
        return ZMK_BEHAVIOR_OPAQUE; /* a snippet was typed on press: nothing to release */
    }
    send(binding->param1, false);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_snippet_driver_api = {
    .binding_pressed = on_pressed,
    .binding_released = on_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_snippet_driver_api);

#elif DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

/* peripheral build: the node must exist, the behavior never runs there */
#include <drivers/behavior.h>
static int nop(struct zmk_behavior_binding *b, struct zmk_behavior_binding_event e) { return 0; }
static const struct behavior_driver_api behavior_snippet_driver_api = {
    .binding_pressed = nop,
    .binding_released = nop,
};
BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_snippet_driver_api);

#endif
