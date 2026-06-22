"""
Automation triggers for Grove Vision AI V2
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation

from .. import grove_vision_ai_ns

DEPENDENCIES = ["grove_vision_ai"]

GroveVisionAIOnDetectionTrigger = grove_vision_ai_ns.class_(
    "GroveVisionAIOnDetectionTrigger", automation.Trigger.template()
)

CONF_ON_DETECTION = "on_detection"

DETECTION_TRIGGER_SCHEMA = automation.TRIGGER_BASE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(GroveVisionAIOnDetectionTrigger),
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ON_DETECTION): automation.validate_automation(
            DETECTION_TRIGGER_SCHEMA
        ),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ON_DETECTION][CONF_ID])
    await cg.register_component(var, config)
    await automation.build_automation(
        var, [(int, "target_id"), (int, "x"), (int, "y"), (int, "w"), (int, "h"), (float, "score")],
        config[CONF_ON_DETECTION],
    )