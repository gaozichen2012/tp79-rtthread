#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x_gpio.h"
#include "device_config.h"

#define ADC1_DR_Address    0x40012440
#define ADC_SAMPLE_COUNT   100			//AD滤波采样次数	

#define ADC_CHANNELS 3
#define ADC_CHANEL_BAT  0
#define ADC_CHANEL_KEY1018  1
#define ADC_CHANEL_KEY19 2

#define BAT_LOW_POWER_VALUE  4052
#define BAT_POWER_VALUE_LEVEL  100

typedef struct
{
	uint8_t  Flag;
	uint16_t Value[ADC_CHANNELS];
}Adc_t,*Adc_p;


void ADC_init(void);

uint16_t get_adc_value(uint8_t adc_chanel);

#endif
