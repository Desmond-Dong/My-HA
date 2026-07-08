# CMakeLists for M5Stack Tab5 camera external component (platform subdirectory).
# ESPHome detects this and compiles as part of the main library.

idf_component_register(
    SRCS "m5_tab5_camera.cpp"
    INCLUDE_DIRS "."
)
