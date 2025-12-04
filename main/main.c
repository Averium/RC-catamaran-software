#include "utils.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "application_state_machine.h"


static float pulse_us = 0.0f;
static Direction dir = INCREASE;
static float step_us;

static StateMachine state_machine;


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

    ESP_LOGI(TAG, "PWM initialized on GPIO %d", PWM_PIN);
}

// convert float µs → LEDC duty count
static inline uint32_t pulse_to_duty(float p) {
    return DUTY_FROM_PULSE_US(p);
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

    ESP_LOGI(TAG, "Starting ESP init sequence...");

    for (float scale = -1.0f; scale < 2.0f; scale = scale + 0.01f) {  // 2 sec at 10ms per update
        
        if (scale < 0.0f) {
            norm = 0.0f;
        } else if (scale > 1.0f) {
            norm = 1.0f;
        } else {
            norm = scale;
            ESP_LOGI(TAG, "Calibration progress: %.1f", (float)(norm * 100.0f));
        }

        uint32_t duty = pulse_to_duty(PULSE_MAX_US - range * norm);

        ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
        ledc_update_duty(PWM_MODE, PWM_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(UPDATE_INTERVAL_MS));
    }
}


void esc_task(void* pv) {
    // compute step size for 5s ramp
    const float ramp_time_s = 20.0f;
    const float updates_per_sec = 1000.0f / (float)UPDATE_INTERVAL_MS;
    step_us = (PULSE_MAX_US - PULSE_MIN_US) / (ramp_time_s * updates_per_sec);

    ESP_LOGI(TAG, "Starting ESC ramp test, step %.3f us per update", step_us);

    // initial neutral pulse for ESC arming
    ESC_init_sequence();

    while (true) {
        update_pwm();
        ESP_LOGI(TAG, "pulse %.4f", pulse_us);
        vTaskDelay(pdMS_TO_TICKS(UPDATE_INTERVAL_MS));
    }
}


void app_main(void) {
    StateMachine_init(&state_machine);

    // pwm_init();
    // xTaskCreate(esc_task, "esc_task", 4096, NULL, 5, NULL);

    while (true) {
        StateMachine_update(&state_machine, UPDATE_INTERVAL_MS);
        StateMachine_print_state(&state_machine);
        vTaskDelay(pdMS_TO_TICKS(UPDATE_INTERVAL_MS));
    }
}
