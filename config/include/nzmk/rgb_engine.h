#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Extended RGB modes: 0 SOL, 1 BRE, 2 SPE, 3 SWI, 4 LAY (the fork's effects) + 5 RIP, 6 INV (ours) */
#define NZMK_RGB_MODE_COUNT 7
#define NZMK_RGB_MODE_LAYER 4
#define NZMK_RGB_MODE_RIPPLE 5
#define NZMK_RGB_MODE_INV 6

int nzmk_rgb_mode(void);        /* current mode 0..6 */
bool nzmk_rgb_is_on(void);      /* RGB on/off as the user sees it */
bool nzmk_rgb_fx_active(void);  /* our engine owns the LEDs right now (effect or battery view) */

void nzmk_rgb_command(uint32_t cmd);        /* central: RGB_*_CMD or 100 = LAY (from &lrgb) */
void nzmk_rgb_apply_sync(uint32_t packed);  /* peripheral: state sent by the central (&lsync) */
void nzmk_rgb_battery_view(bool on);        /* &lbat: show the battery level on the keys */

/* Caps state (host Caps Lock + our Caps Word behaviors) */
#define NZMK_CAPS_NONE 0
#define NZMK_CAPS_LOCK 1
#define NZMK_CAPS_WORD 2
int nzmk_caps_state(void);                     /* NZMK_CAPS_* */
bool nzmk_caps_active(void);
void nzmk_caps_word_set(int which, bool on);   /* which: 0 = EN, 1 = RU */
void nzmk_caps_remote(bool on);                /* peripheral: state pushed by the central */
uint32_t nzmk_caps_color(uint32_t normal_hex); /* red, or cyan when the normal color is already red */
void nzmk_rgb_request_sync(void);              /* central: push the RGB/caps state to the right half */
