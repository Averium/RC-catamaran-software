idf_component_register(
    SRCS
        "bldc_driver/bldc_driver.c"
        "pwm/pwm.c"

    INCLUDE_DIRS
        "bldc_driver"
        "pwm"

    REQUIRES
        utilities
        driver
)