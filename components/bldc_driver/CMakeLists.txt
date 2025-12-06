idf_component_register(
    SRCS
        "bldc_driver.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        utilities
        driver
        esp_platform
)