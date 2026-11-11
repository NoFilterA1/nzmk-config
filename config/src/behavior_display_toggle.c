/*
 * &dsptog: toggle display blanking on the half where the behavior runs.
 * Uses the display's blanking (OLED command), NOT the external power rail.
 */

#define DT_DRV_COMPAT zmk_behavior_display_toggle

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/display.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static bool display_is_blanked;

static void display_toggle_cb(struct k_work *work) {
    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (display_is_blanked) {
        display_blanking_off(display);
    } else {
        display_blanking_on(display);
    }
    display_is_blanked = !display_is_blanked;
}

K_WORK_DEFINE(display_toggle_work, display_toggle_cb);

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    k_work_submit_to_queue(zmk_display_work_q(), &display_toggle_work);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_display_toggle_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_display_toggle_driver_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
bool nzmk_display_blanked(void) { return display_is_blanked; }
#endif
