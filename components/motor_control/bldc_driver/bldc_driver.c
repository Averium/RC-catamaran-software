#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "pwm.h"
#include "bldc_driver.h"
#include "application_config.h"
#include "application_types.h"
#include "application_utils.h"


void bldc_init(BldcDriver* object, gpio_num_t gpio_pin, ledc_channel_t pwm_channel) {
    static bool bldc_timer_initialized = false;

    if (!bldc_timer_initialized) {
        pwm_timer_config(BLDC_PWM_TIMER, BLDC_PWM_RESOLUTION, BLDC_PWM_FREQ_HZ);
        bldc_timer_initialized = true;
    }

    pwm_config(gpio_pin, BLDC_PWM_TIMER, pwm_channel);

    object->pwm_channel = pwm_channel;
    object->throttle = 0.0f;
    object->throttle_limit = (Limitation) { 0.0f, 1.0f };
    object->throttle_rate = BLDC_THROTTLE_RATE_LIMIT;
}


void bldc_calibration_sequence(BldcDriver* object, float32 dt) {

    ESP_LOGI(APPLICATION_TAG, "Starting ESP init sequence...");

    float32 time = BLDC_CALIBRATION_DELAY_S * 2.0f + BLDC_CALIBRATION_TIME_S;
    float32 step = dt / time;

    for (float32 progress = 0.0f; progress < 1.0f; progress += step) {

        float32 throttle = progress * time - BLDC_CALIBRATION_DELAY_S;
        bldc_set_throttle(object, throttle);

        ESP_LOGI(APPLICATION_TAG, "Calibration progress: %.1f", progress);
        vTaskDelay(pdMS_TO_TICKS(REFRESH_PERIOD_MS));
    }
}


void bldc_set_throttle(BldcDriver* object, float32 throttle) {
    object->throttle_target = saturate(throttle, object->throttle_limit);
}


void bldc_update(BldcDriver* object, float32 dt) {

    float32 throttle_error = object->throttle_target - object->throttle;
    float32 thorttle_step = saturate(throttle_error, object->throttle_rate);

    object->throttle += thorttle_step * dt;

    float32 pulse_length_us = object->throttle * (PULSE_MAX_US - PULSE_MIN_US) + PULSE_MIN_US;
    uint32 duty_cycle = (pulse_length_us * BLDC_DUTY_MAX / BLDC_PWM_PERIOD_US);

    pwm_update(object->pwm_channel, duty_cycle);
}