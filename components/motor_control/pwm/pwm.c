#include "pwm.h"

#include "esp_log.h"
#include "driver/ledc.h"

#include "application_types.h"
#include "application_config.h"


void pwm_init(void) {
    ledc_timer_config_t timer_config = {
        .speed_mode = PWM_MODE,
        .timer_num = PWM_TIMER,
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    ledc_channel_config_t channel_config = {
        .gpio_num = PWM_PIN,
        .speed_mode = PWM_MODE,
        .channel = PWM_CHANNEL,
        .timer_sel = PWM_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));

    ESP_LOGI(APPLICATION_TAG, "PWM initialized on GPIO %d", PWM_PIN);
}


void pwm_update(const uint32 duty_cycle) {
    ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty_cycle);
    ledc_update_duty(PWM_MODE, PWM_CHANNEL);
}