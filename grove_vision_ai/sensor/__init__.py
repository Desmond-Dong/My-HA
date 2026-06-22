"""
Sensor component for Grove Vision AI V2
"""
from esphome.components import sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_ICON,
    CONF_ID,
    CONF_NAME,
    STATE_CLASS_MEASUREMENT,
    UNIT_EMPTY,
    UNIT_PERCENT,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from .. import grove_vision_ai_ns

DEPENDENCIES = ["grove_vision_ai"]

GroveVisionAISensor = grove_vision_ai_ns.class_("GroveVisionAISensor", sensor.Sensor, cg.Component)

CONF_DETECTIONS_COUNT = "detections_count"
CONF_PERFORMANCE_TIME = "performance_time"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(GroveVisionAISensor),
        cv.Optional(CONF_NAME, default=""): cv.string,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)