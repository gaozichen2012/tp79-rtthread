/**--------------------------------------------------------------------------------------------------------
** Created by:         pengfei li
** Created date:        2017-08-21
** Version:             V1.00
** Descriptions:             
--------------------------------------------------------------------------------------------------------**/

#ifndef __KEY_H_
#define __KEY_H_

#include "device_config.h"

/****   ADC 按键配置   ****/
#define KEY_ADC_CONFIG_A6A7    //使用PA6，PA7，作为ADC按键输入，每个通道挂9个按键


#ifdef KEY_ADC_CONFIG_A6A7
#define KEY_ADC1_PORT GPIOA
#define KEY_ADC2_PORT GPIOA
#define KEY_ADC1_PIN GPIO_Pin_6
#define KEY_ADC2_PIN GPIO_Pin_7
#endif

/****  //使用PA4，PA5,A11作为普通GPIO输入KEY   ****/

#define KEY_GPIO_CONFIG_A4A5A11  

#ifdef KEY_GPIO_CONFIG_A4A5A11
#define KEY19_PORT GPIOA
#define KEY20_PORT GPIOA
#define KEY21_PORT GPIOA

#define KEY19_PIN GPIO_Pin_4
#define KEY20_PIN GPIO_Pin_5
#define KEY21_PIN GPIO_Pin_11

#endif


#define KEY_TINGLE_TIMER_MAX   20	//按键去抖动时长  ms
#define KEY_TIMER_1S_MAX 			 1000 //长按1秒
#define KEY_TIMER_3S_MAX 			 3000 //长按3秒
typedef enum
{
	KEY_VALUE_NONE, 
	
	KEY_VALUE_DOWN1, 
	KEY_VALUE_DOWN2, 
	KEY_VALUE_DOWN3, 
	KEY_VALUE_DOWN4, 
	KEY_VALUE_DOWN5, 
	KEY_VALUE_DOWN6, 
	KEY_VALUE_DOWN7, 
	KEY_VALUE_DOWN8, 
	KEY_VALUE_DOWN9, 
	KEY_VALUE_DOWN10, 
	KEY_VALUE_DOWN11, 
	KEY_VALUE_DOWN12, 
	KEY_VALUE_DOWN13, 
	KEY_VALUE_DOWN14, 
	KEY_VALUE_DOWN15, 
	KEY_VALUE_DOWN16, 
	KEY_VALUE_DOWN17, 
	KEY_VALUE_DOWN18, 
	
	KEY_VALUE_DOWN19, 
	KEY_VALUE_DOWN20, 
	KEY_VALUE_DOWN21, 
	KEY_VALUE_DOWN1_3S ,	
	KEY_VALUE_DOWN20_3S	= 42,
	KEY_VALUE_DOWN21_3S	,
	
	KEY_VALUE_STATUS_DOWN,
	
	
	KEY_VALUE_UP1 = 0X80|KEY_VALUE_DOWN1, 
	KEY_VALUE_UP2, 
	KEY_VALUE_UP3, 
	KEY_VALUE_UP4,
	KEY_VALUE_UP5,
	KEY_VALUE_UP6,
	KEY_VALUE_UP7,
	KEY_VALUE_UP8,
	KEY_VALUE_UP9,
	KEY_VALUE_UP10,
	KEY_VALUE_UP11,
	KEY_VALUE_UP12,
	KEY_VALUE_UP13,
	KEY_VALUE_UP14,
	KEY_VALUE_UP15,
	KEY_VALUE_UP16,
	KEY_VALUE_UP17,
	KEY_VALUE_UP18,
	
	KEY_VALUE_UP19, 
	KEY_VALUE_UP20, 
	KEY_VALUE_UP21,	
	KEY_VALUE_DOWN20_3S_UP = 170,
	KEY_VALUE_DOWN21_3S_UP	,	
  KEY_VALUE_STATUS_UP,
	
}KEY_VALUE_T;


void key_init(void);
void key_scan_task(void);
KEY_VALUE_T get_key_value(void); //获取键值
	



#endif


