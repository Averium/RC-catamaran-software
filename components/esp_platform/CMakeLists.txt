idf_component_register(
    SRCS
        "pwm/pwm.c"

    INCLUDE_DIRS
        "pwm"

    REQUIRES
        utilities
        driver
)