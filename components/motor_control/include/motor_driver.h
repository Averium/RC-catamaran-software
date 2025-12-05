#ifndef ESC_DRIVER_H
#define ESC_DRIVER_H


#include <stdint.h>

#include "application_types.h"


extern float pulse_us;
extern Direction dir;
extern float step_us;


uint32_t pulse_to_duty(const float pulse);

void pwm_init(void);
void update_pwm(void);
void ESC_init_sequence(void);


#endif