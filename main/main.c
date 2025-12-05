#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "application_config.h"
#include "application_state_machine.h"
#include "application_types.h"
#include "motor_driver.h"


void esc_task(void* pv) {
    // compute step size for 5s ramp
    const float ramp_time_s = 20.0f;
    const float updates_per_sec = 1000.0f / (float)UPDATE_INTERVAL_MS;
    step_us = (PULSE_MAX_US - PULSE_MIN_US) / (ramp_time_s * updates_per_sec);

    ESP_LOGI(APPLICATION_TAG, "Starting ESC ramp test, step %.3f us per update", step_us);

    // initial neutral pulse for ESC arming
    ESC_init_sequence();

    while (true) {
        update_pwm();
        ESP_LOGI(APPLICATION_TAG, "pulse %.4f", pulse_us);
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
