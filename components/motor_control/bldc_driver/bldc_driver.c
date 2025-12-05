#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "pwm.h"
#include "bldc_driver.h"
#include "application_config.h"
#include "application_types.h"


float32 pulse_us = 0.0f;
float32 step_us = 0.0f;
Direction dir = INCREASE;


uint32 pulse_to_duty(const float32 pulse) {
    /* convert float µs → LEDC duty count */
    return (uint32)(pulse * DUTY_MAX / PWM_PERIOD_US);
}


void esc_update(void) {

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

    uint32 duty_cycle = pulse_to_duty(pulse_us);
    pwm_update(duty_cycle);

}


void esc_init_sequence(void) {
    const float32 range = PULSE_MAX_US - PULSE_MIN_US;
    float32 norm = 0.0f;

    ESP_LOGI(APPLICATION_TAG, "Starting ESP init sequence...");

    for (float32 scale = -1.0f; scale < 2.0f; scale = scale + 0.01f) {  // 2 sec at 10ms per update
        
        if (scale < 0.0f) {
            norm = 0.0f;
        } else if (scale > 1.0f) {
            norm = 1.0f;
        } else {
            norm = scale;
            ESP_LOGI(APPLICATION_TAG, "Calibration progress: %.1f", norm * 100.0f);
        }

        uint32 duty = pulse_to_duty(PULSE_MAX_US - range * norm);

        ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
        ledc_update_duty(PWM_MODE, PWM_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(REFRESH_PERIOD_MS));
    }
}