"""
Binary sensor component for Grove Vision AI V2 detection events
"""
from esphome.components import binary_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_ICON,
    CONF_ID,
    CONF_NAME,
    DEVICE_CLASS_MOTION,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from .. import grove_vision_ai_ns

DEPENDENCIES = ["grove_vision_ai"]

GroveVisionAIBinarySensor = grove_vision_ai_ns.class_(
    "GroveVisionAIBinarySensor", binary_sensor.BinarySensor, cg.Component
)

CONF_TARGET_ID = "target_id"
CONF_MIN_CONFIDENCE = "min_confidence"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(GroveVisionAIBinarySensor),
        cv.Optional(CONF_NAME, default=""): cv.string,
        cv.Optional(CONF_TARGET_ID): cv.int_,
        cv.Optional(CONF_MIN_CONFIDENCE, default=0.5): cv.float_,
    }
).extend(binary_sensor.BINARY_SENSOR_SCHEMA).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)
    
    if CONF_TARGET_ID in config:
        cg.add(var.set_target_id(config[CONF_TARGET_ID]))
    
    cg.add(var.set_min_confidence(config[CONF_MIN_CONFIDENCE]))