/*
 * &lflash N: very short, dim flash of the whole LED strip (0 = red, 1 = green).
 * Works even when RGB is off; restores the previous state afterwards.
 */

#define DT_DRV_COMPAT zmk_behavior_led_flash

#include <string.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/workqueue.h>
#include <zmk/rgb_underglow.h>
#include <nzmk/rgb_engine.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) && DT_HAS_CHOSEN(zmk_underglow)

#define STRIP_NODE DT_CHOSEN(zmk_underglow)
#define STRIP_PIXELS DT_PROP(STRIP_NODE, chain_length)

#define FLASH_LEVEL 40 /* 0..255, dim */
#define FLASH_MS 120

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);
static struct led_rgb pixels[STRIP_PIXELS];
static bool was_on;

static void restore_cb(struct k_work *work) {
    if (nzmk_rgb_fx_active()) {
        /* our own effect engine redraws on its next tick */
    } else if (was_on) {
        zmk_rgb_underglow_on(); /* redraws the current layer colors / effect */
    } else {
        memset(pixels, 0, sizeof(pixels));
        led_strip_update_rgb(strip, pixels, STRIP_PIXELS);
    }
}

static K_WORK_DELAYABLE_DEFINE(restore_work, restore_cb);

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    struct led_rgb color = {.r = 0, .g = 0, .b = 0};

    if (binding->param1 == 0) {
        color.r = FLASH_LEVEL;
    } else {
        color.g = FLASH_LEVEL;
    }

    if (!device_is_ready(strip)) {
        return -ENODEV;
    }

    /* only remember the state if no flash is already in progress */
    if (!k_work_delayable_is_pending(&restore_work)) {
        zmk_rgb_underglow_get_state(&was_on);
    }

    for (int i = 0; i < STRIP_PIXELS; i++) {
        pixels[i] = color;
    }
    led_strip_update_rgb(strip, pixels, STRIP_PIXELS);

    k_work_reschedule_for_queue(zmk_workqueue_lowprio_work_q(), &restore_work, K_MSEC(FLASH_MS));
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_led_flash_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_led_flash_driver_api);

#endif
