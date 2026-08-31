import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.esp32 import add_idf_component, add_idf_sdkconfig_option, VARIANT_ESP32P4, only_on_variant
from esphome.core.entity_helpers import setup_entity
from esphome.const import CONF_ID

CODEOWNERS = ["@Desmond-Dong"]

DEPENDENCIES = ["esp32", "i2c"]
AUTO_LOAD = ["camera"]

CONF_RESOLUTION = "resolution"
CONF_JPEG_QUALITY = "jpeg_quality"
CONF_FRAMERATE = "framerate"

m5_tab5_camera_ns = cg.esphome_ns.namespace("m5_tab5_camera")
M5Tab5Camera = m5_tab5_camera_ns.class_("M5Tab5Camera", cg.PollingComponent, cg.EntityBase)
M5Tab5CameraFrameSize = m5_tab5_camera_ns.enum("M5Tab5CameraFrameSize")

FRAME_SIZES = {
    "400X296": M5Tab5CameraFrameSize.M5_TAB5_CAMERA_SIZE_400X296,
    "640X480": M5Tab5CameraFrameSize.M5_TAB5_CAMERA_SIZE_640X480,
    "800X600": M5Tab5CameraFrameSize.M5_TAB5_CAMERA_SIZE_800X600,
    "1024X768": M5Tab5CameraFrameSize.M5_TAB5_CAMERA_SIZE_1024X768,
    "1280X720": M5Tab5CameraFrameSize.M5_TAB5_CAMERA_SIZE_1280X720,
    "1280X960": M5Tab5CameraFrameSize.M5_TAB5_CAMERA_SIZE_1280X960,
    "1600X1200": M5Tab5CameraFrameSize.M5_TAB5_CAMERA_SIZE_1600X1200,
}

CONFIG_SCHEMA = cv.All(
    cv.ENTITY_BASE_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(M5Tab5Camera),
            cv.Optional(CONF_RESOLUTION, default="800X600"): cv.enum(FRAME_SIZES, upper=True),
            cv.Optional(CONF_JPEG_QUALITY, default=12): cv.int_range(min=4, max=63),
            cv.Optional(CONF_FRAMERATE, default=10): cv.int_range(min=1, max=30),
        }
    ).extend(cv.COMPONENT_SCHEMA),
    only_on_variant(supported=[VARIANT_ESP32P4]),
)


async def to_code(config):
    cg.add_define("USE_CAMERA")
    var = cg.new_Pvariable(config[CONF_ID])
    await setup_entity(var, config, "camera")
    await cg.register_component(var, config)

    cg.add(var.set_frame_size(config[CONF_RESOLUTION]))
    cg.add(var.set_jpeg_quality(config[CONF_JPEG_QUALITY]))
    cg.add(var.set_framerate(config[CONF_FRAMERATE]))

    add_idf_component(name="espressif/m5stack_tab5", ref="^1.2.0")
    add_idf_component(name="espressif/esp_ipa", ref="^1.3.0")

    # Hardware JPEG encoder (ESP32-P4 has a dedicated codec)
    add_idf_sdkconfig_option("CONFIG_JPEG_CONTROLLER_ENABLED", True)
    add_idf_sdkconfig_option("CONFIG_JPEG_DECODER_ENABLED", True)
    add_idf_sdkconfig_option("CONFIG_JPEG_ENCODER_ENABLED", True)
