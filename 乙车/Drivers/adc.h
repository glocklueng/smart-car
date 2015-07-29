#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f4xx_adc.h"

extern volatile u16 adcValue[2];
extern volatile uint8_t adc_flag;
extern volatile uint8_t adc_valid_flag;
extern volatile uint16_t adc_threshold;

void adc_config(void);
void adc_start_converte(void);
void adc_get_voltage(volatile u16 *adcValue, float *voltage);

void adc_tim_trig_config(uint32_t period, uint32_t prescaler);

#endif
