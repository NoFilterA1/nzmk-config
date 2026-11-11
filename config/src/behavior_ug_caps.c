/* &ugc 0xRRGGBB: a fixed layer color that turns red (cyan if it is red already) while Caps is on. */

#define DT_DRV_COMPAT zmk_behavior_ug_caps

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <nzmk/rgb_engine.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int ug_caps_process(struct zmk_behavior_binding *binding,
                           struct zmk_behavior_binding_event event) {
    uint32_t hex = binding->param1;
    if (nzmk_caps_active()) {
        hex = nzmk_caps_color(hex);
    }
    return hex > 0 ? (int)hex : 1;
}

static const struct behavior_driver_api ug_caps_driver_api = {
    .binding_pressed = ug_caps_process,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &ug_caps_driver_api);

#endif
