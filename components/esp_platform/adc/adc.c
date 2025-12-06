#include "adc.h"
#include <stdlib.h>
#include <string.h>
#include <esp_log.h>


static const char *TAG = "ADC";


struct AdcWrapper {
    adc1_channel_t channel;
    adc_atten_t atten;
    adc_bits_width_t width;
    esp_adc_cal_characteristics_t characteristics;

    bool is_initialized;
};


AdcWrapper* adc_new(adc1_channel_t channel, adc_atten_t atten, adc_bits_width_t width) {

    AdcWrapper* object = malloc(sizeof(*object));

    if (!object) { return NULL; }

    object->channel = channel;
    object->atten = atten;
    object->width = width;
    object->is_initialized = false;
}


void adc_init(AdcWrapper* object) {

    ESP_ERROR_CHECK(adc1_config_width(object->width));
    ESP_ERROR_CHECK(adc1_config_channel_atten(object->channel, object->atten));

    esp_adc_cal_value_t val_type = (esp_adc_cal_value_t)(0);
    val_type = esp_adc_cal_characterize(ADC_UNIT_1, object->atten, object->width, 0, &object->characteristics);

    ESP_LOGI(
        TAG, "ADC initialized: channel: %d atten: %d width: %d type: %d",
        object->channel, object->atten, object->width, val_type
    );

    object->is_initialized = true;

}


uint32 adc_read_raw(AdcWrapper* adc) {
    if (!adc->is_initialized) { return -1; }

    return (uint32)(adc1_get_raw(adc->channel));
}


uint32 adc_read_voltage_mv(AdcWrapper* adc)
{
    if (!adc->is_initialized) { return 0u; }

    int raw = adc_read_raw(adc);
    if (raw < 0) { return 0u; }

    uint32 voltage = (uint32)(esp_adc_cal_raw_to_voltage((uint32)(raw), &adc->characteristics));
    return voltage;
}
