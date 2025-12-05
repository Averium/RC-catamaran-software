#ifndef PWM_H
#define PWM_H


#include "application_types.h"


void pwm_init(void);
void pwm_update(const uint32 duty_cycle);


#endif