#ifndef APPLICATION_STATE_MACHINE_H
#define APPLICATION_STATE_MACHINE_H


//#include "application_types.h"
#include "esp_log.h"


typedef enum {
    STATE_STARTUP,
    STATE_SHUTDOWN,
    STATE_OPERATION,
    STATE_ERROR,
    N_STATES,
} StateType;


typedef enum {
    EVENT_NONE,
    EVENT_RC_COMMAND,
    EVENT_ERROR,
    N_EVENTS
} EventType;


typedef void (*OnEntryFunction)(void);
typedef void (*OnExitFunction)(void);
typedef void (*OperationFunction)(const float dt);
typedef StateType (*TransitionsFunction)(void);


typedef struct {
    StateType state;

    OnEntryFunction on_entry;
    OnExitFunction on_exit;
    OperationFunction operation;
    TransitionsFunction transitions;
} State;


typedef struct {
    StateType current_state;
    State state_table[N_STATES];
} StateMachine;


void StateMachine_init(StateMachine* state_machine);

void StateMachine_set_state(
    StateMachine* state_machine,
    StateType state,
    OnEntryFunction on_entry,
    OnExitFunction on_exit,
    OperationFunction operation,
    TransitionsFunction transitions
);

void StateMachine_update(StateMachine* state_machine, const float dt);

inline State* StateMachine_get_state(StateMachine* state_machine) {
    const StateType index = state_machine->current_state;
    return &(state_machine->state_table[index]);
}

void StateMachine_print_state(StateMachine* state_machine);

#endif