/*
 * &lrgb <cmd>: RGB controls with HOLD-TO-REPEAT. Runs on the CENTRAL only (the engine pushes the
 * resulting state to the right half, so both halves always match).
 * cmd = RGB_*_CMD from dt-bindings/zmk/rgb.h, or 100 = select the "layer colors" mode.
 */

#define DT_DRV_COMPAT zmk_behavior_led_rgb

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <dt-bindings/zmk/rgb.h>
#include <nzmk/rgb_engine.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

#define REPEAT_DELAY_MS 200
#define REPEAT_PERIOD_MS 80

static uint32_t held_cmd = UINT32_MAX;

static bool is_repeatable(uint32_t cmd) {
    switch (cmd) {
    case RGB_HUI_CMD:
    case RGB_HUD_CMD:
    case RGB_SAI_CMD:
    case RGB_SAD_CMD:
    case RGB_BRI_CMD:
    case RGB_BRD_CMD:
    case RGB_SPI_CMD:
    case RGB_SPD_CMD:
        return true;
    default:
        return false;
    }
}

static void repeat_cb(struct k_work *work) {
    if (held_cmd == UINT32_MAX) {
        return;
    }
    nzmk_rgb_command(held_cmd);
    k_work_reschedule(k_work_delayable_from_work(work), K_MSEC(REPEAT_PERIOD_MS));
}

static K_WORK_DELAYABLE_DEFINE(repeat_work, repeat_cb);

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    const uint32_t cmd = binding->param1;

    nzmk_rgb_command(cmd);

    if (is_repeatable(cmd)) {
        held_cmd = cmd;
        k_work_reschedule(&repeat_work, K_MSEC(REPEAT_DELAY_MS));
    }
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    if (binding->param1 == held_cmd) {
        held_cmd = UINT32_MAX;
        k_work_cancel_delayable(&repeat_work);
    }
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_led_rgb_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    .locality = BEHAVIOR_LOCALITY_CENTRAL,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_led_rgb_driver_api);

#endif
