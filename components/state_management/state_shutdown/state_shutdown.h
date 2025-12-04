#ifndef STATE_SHUTDOWN_H
#define STATE_SHUTDOWN_H

#include "application_state_machine.h"


void on_entry_shutdown(void);
void on_exit_shutdown(void);
void operation_shutdown(const float dt);
StateType transitions_shutdown(void);


#endif