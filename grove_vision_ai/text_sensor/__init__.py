"""
Text sensor component for Grove Vision AI V2 detection results
"""
from esphome.components import text_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_ID,
    CONF_NAME,
)

from .. import grove_vision_ai_ns

DEPENDENCIES = ["grove_vision_ai"]

GroveVisionAITextSensor = grove_vision_ai_ns.class_(
    "GroveVisionAITextSensor", text_sensor.TextSensor, cg.Component
)

CONF_DETECTION_RESULTS = "detection_results"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(GroveVisionAITextSensor),
        cv.Optional(CONF_NAME, default=""): cv.string,
    }
).extend(text_sensor.TEXT_SENSOR_SCHEMA).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)