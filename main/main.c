#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "application_config.h"
#include "application_state_machine.h"
#include "application_types.h"
#include "application_utils.h"
#include "bldc_driver.h"


static BldcDriver* bldc = NULL;
static const char* TAG = "MAIN";


void esc_task(void* pv) {

    static float32 throttle = 0.0f;
    static float32 direction = 1.0f;

    static const float32 sweep_time_s = 5.0f;
    static const float32 sweep_step = (1.0f / sweep_time_s) * (float32)(REFRESH_PERIOD_MS) * MS_TO_S;

    bldc_calibration_sequence(bldc, REFRESH_PERIOD_MS);

    while (true) {

        if (throttle < 0.0f) {
            throttle = 0.0f;
            direction = -direction;
        }

        if (throttle > 1.0f) {
            throttle = 1.0f;
            direction = -direction;
        }

        throttle = throttle + sweep_step * direction;
        bldc_set_throttle(bldc, throttle);

        ESP_LOGI(TAG, "Throttle set to: %.2f%c", throttle * 100.0f, '%');
        
        vTaskDelay(pdMS_TO_TICKS(REFRESH_PERIOD_MS));

    }
}


void app_main(void) {

    bldc = bldc_new(BLDC_PIN_1, BLDC_PWM_CHANNEL_1);
    bldc_init(bldc);

    // StateMachine_init(&state_machine);

    xTaskCreate(esc_task, "esc_task", 4096, NULL, 5, NULL);

    while (true) {
        // StateMachine_update(&state_machine, REFRESH_PERIOD_MS);
        // StateMachine_print_state(&state_machine);
        vTaskDelay(pdMS_TO_TICKS(REFRESH_PERIOD_MS));
    }
}
