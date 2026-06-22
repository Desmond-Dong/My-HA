set(COMPONENT_SRCS
  "grove_vision_ai.cpp"
  "sensor/sensor.cpp"
  "binary_sensor/binary_sensor.cpp"
  "text_sensor/text_sensor.cpp"
  "automation/automation.cpp"
)

if(USE_GROVE_VISION_AI_VIDEO_STREAM)
  list(APPEND COMPONENT_SRCS "video_stream/video_stream.cpp")
endif()

set(COMPONENT_ADD_INCLUDEDIRS "." "sensor" "binary_sensor" "text_sensor" "automation" "video_stream")

register_component()