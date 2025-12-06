idf_component_register(
    SRCS
        "battery_monitor.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        utilities
        esp_platform
)