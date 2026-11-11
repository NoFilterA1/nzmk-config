/*
 * &ugs: "solid" color for the per-layer RGB (LAY mode). Returns the CURRENT solid color
 * (hue/saturation set with the RGB keys) at full value; the fork then scales it by the
 * brightness, exactly like the other layer colors. Use it in the rgb_layer bindings.
 */

#define DT_DRV_COMPAT zmk_behavior_ug_solid

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/rgb_underglow.h>
#include <nzmk/rgb_engine.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int ug_solid_init(const struct device *dev) { return 0; }

static int ug_solid_process(struct zmk_behavior_binding *binding,
                            struct zmk_behavior_binding_event event) {
    const struct zmk_led_hsb c = zmk_rgb_underglow_calc_brt(0); /* direction 0 = current value */

    const float s = c.s / 100.0f;
    const int i = c.h / 60;
    const float f = c.h / 60.0f - i;
    const float p = 1.0f - s;
    const float q = 1.0f - f * s;
    const float t = 1.0f - (1.0f - f) * s;

    float r = 0, g = 0, b = 0;
    switch (i % 6) {
    case 0: r = 1; g = t; b = p; break;
    case 1: r = q; g = 1; b = p; break;
    case 2: r = p; g = 1; b = t; break;
    case 3: r = p; g = q; b = 1; break;
    case 4: r = t; g = p; b = 1; break;
    default: r = 1; g = p; b = q; break;
    }

    int hex = (((int)(r * 255.0f)) << 16) | (((int)(g * 255.0f)) << 8) | (int)(b * 255.0f);
    if (nzmk_caps_active()) {
        hex = nzmk_caps_color(hex); /* Caps on: keys turn red (cyan if the color is already red) */
    }
    return hex > 0 ? hex : 1; /* the fork treats 0 as "LED off" */
}

static const struct behavior_driver_api ug_solid_driver_api = {
    .binding_pressed = ug_solid_process,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

BEHAVIOR_DT_INST_DEFINE(0, ug_solid_init, NULL, NULL, NULL, POST_KERNEL,
                        CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &ug_solid_driver_api);

#endif
