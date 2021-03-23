#ifndef __LED_H_
#define __LED_H_

#include "device_config.h"

#define LED_PIN_F0F1   //¶¨ÒåLEDÎªF0F1

#ifdef LED_PIN_F0F1
#define LED_MAX 2
#define LED1_PORT GPIOF
#define LED2_PORT GPIOF
#define LED1_PIN GPIO_Pin_0
#define LED2_PIN GPIO_Pin_1
#endif

typedef enum
{
	LED1,
	LED2,
}led_list_t;

typedef struct
{
	led_list_t  LEDn;
	uint8_t  led_blink_en;
	uint16_t	 led_blink_timer1;
	uint16_t	 led_blink_timer2;	
	
}led_device_t,*led_device_pt;


void LED_init(void);

void LED_ON(led_list_t LEDn);
void LED_OFF(led_list_t LEDn);
uint8_t LED_READ(led_list_t LEDn);
void LED_blink(led_list_t LEDn, uint16_t timeron,uint16_t timeroff);	//led blink  timer = ms
void LED_task(void);
#endif
