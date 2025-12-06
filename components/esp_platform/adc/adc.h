#ifndef ADC_WRAPPER_H
#define ADC_WRAPPER_H

#include <stdbool.h>
#include <esp_err.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

#include "application_types.h"


typedef struct AdcWrapper AdcWrapper;


AdcWrapper* adc_new(adc1_channel_t channel, adc_atten_t atten, adc_bits_width_t width);

void adc_init(AdcWrapper* adc);

uint32 adc_read_raw(AdcWrapper* adc);

uint32 adc_read_voltage_mv(AdcWrapper* adc);


#endif
