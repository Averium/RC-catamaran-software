#ifndef STATE_OPERATION_H
#define STATE_OPERATION_H

#include "application_state_machine.h"


void on_entry_operation(void);
void on_exit_operation(void);
void operation_operation(const float dt);
StateType transitions_operation(void);


#endif