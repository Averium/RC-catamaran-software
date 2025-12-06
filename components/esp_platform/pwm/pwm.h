#ifndef PWM_H
#define PWM_H


#include "driver/ledc.h"

#include "application_types.h"


void pwm_timer_config(ledc_timer_t timer_id, ledc_timer_bit_t resolution, uint32 frequency);
void pwm_config(gpio_num_t gpio_number, ledc_timer_t timer_id, ledc_channel_t channel);
void pwm_update(ledc_channel_t channel, uint32 duty_cycle);


#endif