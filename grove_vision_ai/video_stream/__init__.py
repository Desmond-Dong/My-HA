"""
Video stream component for Grove Vision AI V2
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.const import CONF_ID, CONF_PORT

from .. import grove_vision_ai_ns

DEPENDENCIES = ["grove_vision_ai", "web_server"]

CONF_VIDEO_STREAM = "video_stream"
CONF_PORT = "port"

GroveVisionAIVideoStream = grove_vision_ai_ns.class_(
    "GroveVisionAIVideoStream", cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(GroveVisionAIVideoStream),
        cv.Optional(CONF_PORT, default=8080): cv.port,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    cg.add_define("USE_GROVE_VISION_AI_VIDEO_STREAM")
    cg.add_define("GROVE_VISION_AI_STREAM_PORT", config[CONF_PORT])