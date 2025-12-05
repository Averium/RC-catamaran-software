idf_component_register(
    SRCS
        "application_state_machine/application_state_machine.c"
        "state_startup/state_startup.c"
        "state_shutdown/state_shutdown.c"
        "state_operation/state_operation.c"

    INCLUDE_DIRS
        "application_state_machine"
        "state_startup"
        "state_shutdown"
        "state_operation"

    REQUIRES
        utilities
)