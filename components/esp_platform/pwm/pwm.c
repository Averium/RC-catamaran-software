#include "pwm.h"

#include "esp_log.h"
#include "driver/ledc.h"

#include "application_types.h"
#include "application_config.h"


static const char* TAG = "PWM";


void pwm_timer_config(ledc_timer_t timer_id, ledc_timer_bit_t resolution, uint32 frequency) {

    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = timer_id,
        .duty_resolution = resolution,
        .freq_hz = frequency,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));
}


void pwm_config(
    gpio_num_t gpio_number,
    ledc_timer_t timer_id,
    ledc_channel_t channel
) {

    ledc_channel_config_t channel_config = {
        .gpio_num = gpio_number,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = channel,
        .timer_sel = timer_id,
        .duty = 0,
        .hpoint = 0,
        .intr_type = LEDC_INTR_DISABLE
    };

    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));
    ESP_LOGI(TAG, "PWM initialized on GPIO %d", gpio_number);
}


void pwm_update(ledc_channel_t channel, uint32 duty_cycle) {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty_cycle);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}