#include "state_shutdown.h"


void on_entry_shutdown(void) {}


void on_exit_shutdown(void) {}


void operation_shutdown(const float dt) {}


StateType transitions_shutdown(void) {
    return STATE_OPERATION;
}