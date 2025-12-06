#ifndef BLDC_DRIVER_H
#define BLDC_DRIVER_H


#include <stdint.h>

#include "driver/ledc.h"

#include "application_types.h"
#include "application_config.h"


#define BLDC_PWM_TIMER      LEDC_TIMER_0
#define BLDC_PWM_MODE       LEDC_HIGH_SPEED_MODE
#define BLDC_PWM_FREQ_HZ    50u
#define BLDC_PWM_RESOLUTION LEDC_TIMER_12_BIT

#define BLDC_CALIBRATION_DELAY_S 1.0f
#define BLDC_CALIBRATION_TIME_S 1.0f

#define BLDC_THROTTLE_RATE_LIMIT ((Limitation){ 0.0f, 1.0f })

#define BLDC_PWM_PERIOD_US   (1000000.0f / (float32)(BLDC_PWM_FREQ_HZ))
#define BLDC_DUTY_MAX        ((1u << BLDC_PWM_RESOLUTION) - 1u)


typedef struct {

    float32 throttle;
    float32 throttle_target;

    Limitation throttle_rate;
    Limitation throttle_limit;

    ledc_channel_t pwm_channel;

} BldcDriver;


void bldc_init(BldcDriver* object, gpio_num_t gpio_pin, ledc_channel_t pwm_channel);
void bldc_calibration_sequence(BldcDriver* object, float32 dt);
void bldc_set_throttle(BldcDriver* object, float32 throttle);
void bldc_update(BldcDriver* object, float32 dt);


#endif