#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdbool.h>

// ---------- PWM configuration ----------
#define PWM_PIN         GPIO_NUM_4
#define PWM_FREQ_HZ     50u             // 50 Hz
#define PWM_TIMER       LEDC_TIMER_0
#define PWM_MODE        LEDC_HIGH_SPEED_MODE
#define PWM_CHANNEL     LEDC_CHANNEL_0
#define PWM_RESOLUTION  LEDC_TIMER_12_BIT

// ---------- ESC pulse widths in microseconds ----------
#define PULSE_MIN_US    1300.0f
#define PULSE_MAX_US    1800.0f

// ---------- Derived values ----------
#define PWM_PERIOD_US   (1000000.0f / (float)PWM_FREQ_HZ)
#define DUTY_MAX        ((1U << PWM_RESOLUTION) - 1U)
#define DUTY_FROM_PULSE_US(p) ((uint32_t)(((p) * DUTY_MAX) / PWM_PERIOD_US))

#define UPDATE_INTERVAL_MS 10u  // 10 ms update interval

#define TAG "ESP: "

typedef enum { INCREASE, DECREASE } Direction;

#endif
