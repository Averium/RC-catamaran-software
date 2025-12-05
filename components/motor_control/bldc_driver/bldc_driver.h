#ifndef BLDC_DRIVER_H
#define BLDC_DRIVER_H


#include <stdint.h>

#include "application_types.h"


extern float32 pulse_us;
extern float32 step_us;
extern Direction dir;


uint32 pulse_to_duty(const float32 pulse);

void esc_update(void);
void esc_init_sequence(void);


#endif