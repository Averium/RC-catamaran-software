#ifndef STATE_STARTUP_H
#define STATE_STARTUP_H

#include "application_state_machine.h"


void on_entry_startup(void);
void on_exit_startup(void);
void operation_startup(const float32 dt);
StateType transitions_startup(void);


#endif