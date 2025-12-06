#ifndef APPLICATION_CONFIG_H
#define APPLICATION_CONFIG_H


// ---------- PWM configuration ----------
#define BLDC_PIN_1         GPIO_NUM_4
#define BLDC_PWM_CHANNEL_1 LEDC_CHANNEL_0

// ---------- ESC pulse widths in microseconds ----------
#define PULSE_MIN_US    1300.0f
#define PULSE_MAX_US    1800.0f

#define REFRESH_PERIOD_MS 10u
#define APPLICATION_TAG "APPLICATION: "


#endif