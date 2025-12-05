#include "state_startup.h"


void on_entry_startup(void) {}


void on_exit_startup(void) {}


void operation_startup(const float32 dt) {}


StateType transitions_startup(void) {
    static uint8 counter = 0u;

    if (counter > 200u) { return STATE_OPERATION; }
    else { counter++; }

    return STATE_STARTUP;
}