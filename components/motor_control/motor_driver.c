#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "motor_driver.h"
#include "application_config.h"


float pulse_us = 0.0f;
float step_us = 0.0f;
Direction dir = INCREASE;


void pwm_init(void) {
    ledc_timer_config_t timer_cfg = {
        .speed_mode = PWM_MODE,
        .timer_num = PWM_TIMER,
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_cfg));

    ledc_channel_config_t ch_cfg = {
        .gpio_num = PWM_PIN,
        .speed_mode = PWM_MODE,
        .channel = PWM_CHANNEL,
        .timer_sel = PWM_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ch_cfg));

    ESP_LOGI(APPLICATION_TAG, "PWM initialized on GPIO %d", PWM_PIN);
}


uint32_t pulse_to_duty(const float pulse) {
    /* convert float µs → LEDC duty count */
    return (uint32_t)(pulse * DUTY_MAX / PWM_PERIOD_US);
}


void update_pwm(void) {
    if (dir == INCREASE) {
        pulse_us += step_us;
    } else {
        pulse_us -= step_us;
    }

    if (pulse_us >= PULSE_MAX_US) {
        pulse_us = PULSE_MAX_US;
        dir = DECREASE;
    } else if (pulse_us <= PULSE_MIN_US) {
        pulse_us = PULSE_MIN_US;
        dir = INCREASE;
    }

    uint32_t duty = pulse_to_duty(pulse_us);
    ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
    ledc_update_duty(PWM_MODE, PWM_CHANNEL);
}


void ESC_init_sequence(void) {
    const float range = PULSE_MAX_US - PULSE_MIN_US;
    float norm = 0.0f;

    ESP_LOGI(APPLICATION_TAG, "Starting ESP init sequence...");

    for (float scale = -1.0f; scale < 2.0f; scale = scale + 0.01f) {  // 2 sec at 10ms per update
        
        if (scale < 0.0f) {
            norm = 0.0f;
        } else if (scale > 1.0f) {
            norm = 1.0f;
        } else {
            norm = scale;
            ESP_LOGI(APPLICATION_TAG, "Calibration progress: %.1f", (float)(norm * 100.0f));
        }

        uint32_t duty = pulse_to_duty(PULSE_MAX_US - range * norm);

        ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
        ledc_update_duty(PWM_MODE, PWM_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(UPDATE_INTERVAL_MS));
    }
}