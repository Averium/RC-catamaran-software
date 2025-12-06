#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "pwm.h"
#include "bldc_driver.h"
#include "application_config.h"
#include "application_types.h"
#include "application_utils.h"


static const char* TAG = "BLDC";


struct BldcDriver {

    float32 throttle;
    float32 throttle_target;

    Limitation throttle_rate_s;
    Limitation throttle_limit;

    gpio_num_t pwm_pin;
    ledc_channel_t pwm_channel;

    bool is_initialized;
    ApplicationCalibrationState calibration_state;

};


BldcDriver* bldc_new(gpio_num_t gpio_pin, ledc_channel_t pwm_channel) {

    BldcDriver* object = malloc(sizeof(*object));
    
    object->pwm_channel = pwm_channel;
    object->pwm_pin = gpio_pin;

    object->throttle = 0.0f;
    object->throttle_limit = (Limitation) { 0.0f, 1.0f };
    object->throttle_rate_s = BLDC_THROTTLE_RATE_LIMIT_S;

    object->is_initialized = false;
    object->calibration_state = CALIBRATION_NOT_STARTED;

    return object;
}


void bldc_init(BldcDriver* object) {

    static bool bldc_timer_initialized = false;

    if (false == bldc_timer_initialized) {
        pwm_timer_config(BLDC_PWM_TIMER, BLDC_PWM_RESOLUTION, BLDC_PWM_FREQ_HZ);
        bldc_timer_initialized = true;

        ESP_LOGI(TAG, "BLDC PWM timer: %d configured", (uint8)(BLDC_PWM_TIMER));
    }

    pwm_config(object->pwm_pin, BLDC_PWM_TIMER, object->pwm_channel);
    ESP_LOGI(TAG, "PWM signal configured on GPIO: %d", (uint8)(object->pwm_pin));

    object->is_initialized = true;
}


void bldc_calibration_sequence(BldcDriver* object, float32 dt_ms) {

    if (false == object->is_initialized) {
        bldc_init(object);
    }

    if (object->calibration_state == CALIBRATION_FINISHED) {
        return;
    }

    ESP_LOGI(TAG, "Starting ESP calibration sequence...");
    object->calibration_state = CALIBRATION_ONGOING;

    bldc_immediate_throttle(object, 1.0f);

    float32 calibration_time_s = BLDC_CALIBRATION_DELAY_S * 2.0f + BLDC_CALIBRATION_TIME_S;
    float32 step = dt_ms / (calibration_time_s * S_TO_MS);

    for (float32 progress = 0.0f; progress < 1.0f; progress += step) {

        float32 throttle = (1.0f - progress) * calibration_time_s - BLDC_CALIBRATION_DELAY_S;
        bldc_immediate_throttle(object, throttle);
        bldc_update(object, REFRESH_PERIOD_MS);

        ESP_LOGI(TAG, "Calibration progress: %.1f%c", progress * 100.0f, '%');
        vTaskDelay(pdMS_TO_TICKS(REFRESH_PERIOD_MS));
    }

    ESP_LOGI(TAG, "ESP calibration finished.");

    object->calibration_state = CALIBRATION_FINISHED;
}


void bldc_set_throttle(BldcDriver* object, float32 throttle) {
    if (false == (object->is_initialized && object->calibration_state == CALIBRATION_FINISHED)) {
        object->throttle_target = 0.0f;
        return;
    }
    object->throttle_target = saturate(throttle, object->throttle_limit);
}


void bldc_immediate_throttle(BldcDriver* object, float32 throttle) {
    
    if (false == (object->is_initialized && object->calibration_state <= CALIBRATION_ONGOING )) {
        object->throttle = 0.0f;
        object->throttle_target = 0.0f;
        return;
    }
    
    float32 saturated_throttle = saturate(throttle, object->throttle_limit); 
    object->throttle = saturated_throttle;
    object->throttle_target = saturated_throttle;
}


void bldc_update(BldcDriver* object, float32 dt_ms) {

    uint32 duty_cycle = 0u;

    if (false == object->is_initialized) {
        object->throttle = 0.0f;
        return;
    }

    if (object->calibration_state <= CALIBRATION_ONGOING) {
        float32 throttle_error = object->throttle_target - object->throttle;
        float32 thorttle_step = saturate(throttle_error, object->throttle_rate_s);

        object->throttle += thorttle_step * dt_ms * MS_TO_S;

        float32 pulse_length_us = object->throttle * (PULSE_MAX_US - PULSE_MIN_US) + PULSE_MIN_US;
        duty_cycle = (pulse_length_us * BLDC_DUTY_MAX / BLDC_PWM_PERIOD_US);

        ESP_LOGI(TAG, "Pulse length [us]: %.2f", pulse_length_us);
    }

    pwm_update(object->pwm_channel, duty_cycle);
}