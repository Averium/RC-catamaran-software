#include "application_state_machine.h"
#include "application_types.h"

#include "state_startup.h"
#include "state_shutdown.h"
#include "state_operation.h"


StateMachine state_machine;


void StateMachine_init(StateMachine* state_machine) {

    StateMachine_set_state(
        state_machine, STATE_STARTUP,
        on_entry_startup, on_exit_startup, operation_startup, transitions_startup
    );

    StateMachine_set_state(
        state_machine, STATE_SHUTDOWN,
        on_entry_shutdown, on_exit_shutdown, operation_shutdown, transitions_shutdown
    );

    StateMachine_set_state(
        state_machine, STATE_OPERATION,
        on_entry_operation, on_exit_operation, operation_operation, transitions_operation
    );
    
    state_machine->current_state = STATE_STARTUP;
}


void StateMachine_set_state(
    StateMachine* state_machine,
    StateType state,
    OnEntryFunction on_entry,
    OnExitFunction on_exit,
    OperationFunction operation,
    TransitionsFunction transitions
) {
    state_machine->state_table[state] = (State){ state, on_entry, on_exit, operation, transitions };
}


void StateMachine_update(StateMachine* state_machine, const float32 dt) {

    State* state = StateMachine_get_state(state_machine);

    const StateType next_state = state->transitions();
    const StateType current_state = state_machine->current_state;

    if (next_state != current_state) {
        state->on_exit();
        state_machine->current_state = next_state;
        state = StateMachine_get_state(state_machine);
        state->on_entry();
    }

    state->operation(dt);
}


void StateMachine_print_state(StateMachine* state_machine) {

    switch (state_machine->current_state) {
        case STATE_STARTUP:
            ESP_LOGI("STATE MACHINE:", "current state: %s", "STARTUP"); return;
        case STATE_SHUTDOWN:
            ESP_LOGI("STATE MACHINE:", "current state: %s", "SHUTDOWN"); return;
        case STATE_OPERATION:
            ESP_LOGI("STATE MACHINE:", "current state: %s", "OPERATION"); return;
        default:
            ESP_LOGI("STATE MACHINE:", "current state: %s", "UNKNOWN"); return;
    }
}