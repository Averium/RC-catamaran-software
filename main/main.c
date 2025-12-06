#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "application_config.h"
#include "application_state_machine.h"
#include "application_types.h"
#include "bldc_driver.h"


static BldcDriver bldc;


void esc_task(void* pv) {
    bldc_calibration_sequence(&bldc, REFRESH_PERIOD_MS);
}


void app_main(void) {
    StateMachine_init(&state_machine);

    // pwm_init();
    // xTaskCreate(esc_task, "esc_task", 4096, NULL, 5, NULL);

    while (true) {
        StateMachine_update(&state_machine, REFRESH_PERIOD_MS);
        StateMachine_print_state(&state_machine);
        vTaskDelay(pdMS_TO_TICKS(REFRESH_PERIOD_MS));
    }
}
