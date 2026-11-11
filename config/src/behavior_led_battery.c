/* &lbat: while held, every key LED becomes a battery block (green = charge, red = empty). */

#define DT_DRV_COMPAT zmk_behavior_led_battery

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <nzmk/rgb_engine.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int on_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    nzmk_rgb_battery_view(true);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    nzmk_rgb_battery_view(false);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_led_battery_driver_api = {
    .binding_pressed = on_pressed,
    .binding_released = on_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_led_battery_driver_api);

#endif
