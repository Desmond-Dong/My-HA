from esphome import pins
import esphome.codegen as cg
from esphome.components import display
from esphome.components.const import BYTE_ORDER_BIG, BYTE_ORDER_LITTLE, CONF_BYTE_ORDER, CONF_DRAW_ROUNDING
from esphome.components.display import CONF_SHOW_TEST_CARD
from esphome.components.esp32 import VARIANT_ESP32P4, only_on_variant
from esphome.components.mipi import (
    COLOR_ORDERS,
    CONF_COLOR_DEPTH,
    CONF_HSYNC_BACK_PORCH,
    CONF_HSYNC_FRONT_PORCH,
    CONF_HSYNC_PULSE_WIDTH,
    CONF_PCLK_FREQUENCY,
    CONF_PIXEL_MODE,
    CONF_VSYNC_BACK_PORCH,
    CONF_VSYNC_FRONT_PORCH,
    CONF_VSYNC_PULSE_WIDTH,
    PIXEL_MODE_16BIT,
    PIXEL_MODE_24BIT,
    dimension_schema,
    get_color_depth,
    power_of_two,
    requires_buffer,
)
import esphome.config_validation as cv
from esphome.const import (
    CONF_COLOR_ORDER,
    CONF_DIMENSIONS,
    CONF_ENABLE_PIN,
    CONF_ID,
    CONF_INVERT_COLORS,
    CONF_LAMBDA,
    CONF_RESET_PIN,
)
from esphome.final_validate import full_config

from .. import st7123_ns

DEPENDENCIES = ["esp32", "esp_ldo", "psram"]

ST7123Display = st7123_ns.class_("ST7123Display", display.Display, cg.Component)
ColorBitness = display.display_ns.enum("ColorBitness")

CONF_LANE_BIT_RATE = "lane_bit_rate"
CONF_LANES = "lanes"

COLOR_DEPTHS = {16: ColorBitness.COLOR_BITNESS_565, 24: ColorBitness.COLOR_BITNESS_888}

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(ST7123Display),
            cv.Optional(CONF_DIMENSIONS, default={"width": 720, "height": 1280}): dimension_schema(1),
            cv.Optional(CONF_RESET_PIN, default={"pi4ioe5v6408": "pi4ioe1", "number": 4}): pins.gpio_output_pin_schema,
            cv.Optional(CONF_ENABLE_PIN, default=[{"pi4ioe5v6408": "pi4ioe1", "number": 4}]): cv.ensure_list(
                pins.gpio_output_pin_schema
            ),
            cv.Optional(CONF_COLOR_ORDER, default="RGB"): cv.enum(COLOR_ORDERS, upper=True),
            cv.Optional(CONF_DRAW_ROUNDING, default=2): power_of_two,
            cv.Optional(CONF_PIXEL_MODE, default=PIXEL_MODE_16BIT): cv.one_of(
                PIXEL_MODE_16BIT, PIXEL_MODE_24BIT, "16", "24", lower=True
            ),
            cv.Optional(CONF_INVERT_COLORS, default=False): cv.boolean,
            cv.Optional(CONF_COLOR_DEPTH, default="16"): cv.one_of("16", "16bit", "24", "24bit", lower=True),
            cv.Optional(CONF_PCLK_FREQUENCY, default="70MHz"): cv.All(cv.frequency, cv.Range(min=4e6, max=100e6)),
            cv.Optional(CONF_LANES, default=2): cv.int_range(1, 2),
            cv.Optional(CONF_LANE_BIT_RATE, default="1000Mbps"): cv.All(cv.bps, cv.Range(min=100e6, max=3200e6)),
            cv.Optional(CONF_BYTE_ORDER, default=BYTE_ORDER_LITTLE): cv.one_of(BYTE_ORDER_LITTLE, BYTE_ORDER_BIG, lower=True),
            cv.Optional(CONF_HSYNC_PULSE_WIDTH, default=2): cv.int_,
            cv.Optional(CONF_HSYNC_BACK_PORCH, default=40): cv.int_,
            cv.Optional(CONF_HSYNC_FRONT_PORCH, default=40): cv.int_,
            cv.Optional(CONF_VSYNC_PULSE_WIDTH, default=2): cv.int_,
            cv.Optional(CONF_VSYNC_BACK_PORCH, default=8): cv.int_,
            cv.Optional(CONF_VSYNC_FRONT_PORCH, default=220): cv.int_,
        }
    ),
    cv.only_on_esp32,
    only_on_variant(supported=[VARIANT_ESP32P4]),
)


def _final_validate(config):
    global_config = full_config.get()
    from esphome.components.lvgl import DOMAIN as LVGL_DOMAIN
    if not requires_buffer(config) and LVGL_DOMAIN not in global_config:
        config[CONF_SHOW_TEST_CARD] = True
    return config


FINAL_VALIDATE_SCHEMA = _final_validate


async def to_code(config):
    color_depth = COLOR_DEPTHS[get_color_depth(config)]
    pixel_mode = int(config[CONF_PIXEL_MODE].removesuffix("bit"))
    width = config[CONF_DIMENSIONS]["width"]
    height = config[CONF_DIMENSIONS]["height"]
    var = cg.new_Pvariable(config[CONF_ID], width, height, color_depth, pixel_mode)

    cg.add(var.set_invert_colors(config[CONF_INVERT_COLORS]))
    cg.add(var.set_hsync_pulse_width(config[CONF_HSYNC_PULSE_WIDTH]))
    cg.add(var.set_hsync_back_porch(config[CONF_HSYNC_BACK_PORCH]))
    cg.add(var.set_hsync_front_porch(config[CONF_HSYNC_FRONT_PORCH]))
    cg.add(var.set_vsync_pulse_width(config[CONF_VSYNC_PULSE_WIDTH]))
    cg.add(var.set_vsync_back_porch(config[CONF_VSYNC_BACK_PORCH]))
    cg.add(var.set_vsync_front_porch(config[CONF_VSYNC_FRONT_PORCH]))
    cg.add(var.set_pclk_frequency(config[CONF_PCLK_FREQUENCY] / 1.0e6))
    cg.add(var.set_lanes(int(config[CONF_LANES])))
    cg.add(var.set_lane_bit_rate(config[CONF_LANE_BIT_RATE] / 1.0e6))

    reset = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
    cg.add(var.set_reset_pin(reset))

    enable = [await cg.gpio_pin_expression(pin) for pin in config[CONF_ENABLE_PIN]]
    cg.add(var.set_enable_pins(enable))

    await display.register_display(var, config)
    if lamb := config.get(CONF_LAMBDA):
        lambda_ = await cg.process_lambda(lamb, [(display.DisplayRef, "it")], return_type=cg.void)
        cg.add(var.set_writer(lambda_))
