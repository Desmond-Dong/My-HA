"""
ESPHome component for Grove Vision AI V2 (Seeed_Arduino_SSCMA library)
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart
from esphome.const import CONF_ID, CONF_TRIGGER_ID, CONF_UPDATE_INTERVAL

CODEOWNERS = ["@github-user"]

DEPENDENCIES = ["uart"]

AUTO_LOAD = ["sensor", "binary_sensor", "text_sensor", "automation", "video_stream"]

MULTI_CONF = True

grove_vision_ai_ns = cg.esphome_ns.namespace("grove_vision_ai")

GroveVisionAI = grove_vision_ai_ns.class_("GroveVisionAI", cg.Component, uart.UARTDevice)

CONF_UPDATE_INTERVAL = "update_interval"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(GroveVisionAI),
        cv.Optional(CONF_UPDATE_INTERVAL, default="1000ms"): cv.update_interval,
    }
).extend(uart.UART_DEVICE_SCHEMA).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))