/**--------------------------------------------------------------------------------------------------------
** Created by:         pengfei li
** Created date:        2017-08-21
** Version:             V1.00
** Descriptions:             
--------------------------------------------------------------------------------------------------------**/


#include "stm32f10x.h"
#include "key.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#ifdef KEY_ADC_CONFIG_A6A7
#include "adc.h"
#endif



KEY_VALUE_T key_value;
extern uint32_t SysTick_Cnt ;

// PA4,PA5,PA11   GPIO INPUT
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef KEY_GPIO_CONFIG_A4A5A11
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE); 	
	GPIO_InitStructure.GPIO_Pin = KEY19_PIN | KEY20_PIN | KEY21_PIN ;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  
	GPIO_SetBits(KEY19_PORT, KEY19_PIN);	 
	GPIO_SetBits(KEY20_PORT, KEY20_PIN);	 
	GPIO_SetBits(KEY21_PORT, KEY21_PIN);	 
#endif
			
	
}


//获取按键adc  lev等级，转化成key_value

static  KEY_VALUE_T key_get_adc_lev()
{
	KEY_VALUE_T value = KEY_VALUE_NONE;
	uint8_t i;
	const uint16_t tab[9] = {0X0E91,0X0D65,0X0C5B,0X0B40,0X09BB,0X083B,0X0707,0X057E,0X0409};
	uint16_t key_adc_value;
	key_adc_value = get_adc_value(ADC_CHANEL_KEY19);
	for(i=0; i<9; i++)
	{
		if(key_adc_value > 0xffd-0xA8)break;
		if(key_adc_value > tab[i] -  0xA8)
		{
			value = (KEY_VALUE_T)(KEY_VALUE_NONE + i + 1);
			break;
		}
	}
	if(value != KEY_VALUE_NONE)
		return value;
	
	key_adc_value = get_adc_value(ADC_CHANEL_KEY1018);
	for(i=0; i<9; i++)
	{
		if(key_adc_value > 0xffd-0xA8)break;
		if(key_adc_value > 0xE91 - 0X150*i - 0xA8)
		{
			value =(KEY_VALUE_T)( KEY_VALUE_NONE + 9 + i + 1);
			break;
		}
	}	
	if(value <= KEY_VALUE_DOWN9)
		return KEY_VALUE_NONE;
	return value;	
}

//判断是否有按键按下
static uint8_t key_status_isdown(void)
{
	KEY_VALUE_T value = key_get_adc_lev();			
	if(value)
		return value;
#ifdef KEY_GPIO_CONFIG_A4A5A11	
	if( GPIO_ReadInputDataBit(KEY19_PORT,KEY19_PIN ) == 0 )
		return KEY_VALUE_DOWN19;
	else if(GPIO_ReadInputDataBit(KEY20_PORT,KEY20_PIN ) == 0)
		return KEY_VALUE_DOWN20;
	else if(GPIO_ReadInputDataBit(KEY21_PORT,KEY21_PIN ) == 0)
		return KEY_VALUE_DOWN21;
#endif
	return 0;
}
void key_scan_task()
{
	static uint32_t key_tingle_timer;
	volatile  static KEY_VALUE_T key_status = KEY_VALUE_NONE;	
	volatile  static KEY_VALUE_T key_value_temp = KEY_VALUE_NONE;
	if(key_value_temp == KEY_VALUE_NONE) 
	{
		if( key_status_isdown() )
		{
			key_value_temp = key_get_adc_lev();
			if(key_value_temp == KEY_VALUE_NONE)
			{
#ifdef KEY_GPIO_CONFIG_A4A5A11				
				if( GPIO_ReadInputDataBit(KEY19_PORT,KEY19_PIN ) == 0)
				{
					key_value_temp = KEY_VALUE_DOWN19;
				}
				if( GPIO_ReadInputDataBit(KEY20_PORT,KEY20_PIN ) == 0)
				{
					key_value_temp = KEY_VALUE_DOWN20;
				}
				if( GPIO_ReadInputDataBit(KEY21_PORT,KEY21_PIN ) == 0)
				{
					key_value_temp = KEY_VALUE_DOWN21;
				}		
#endif				
			}
			key_tingle_timer = SysTick_Cnt;
		}
	}
	else 
	{
		if(SysTick_Cnt - key_tingle_timer > KEY_TINGLE_TIMER_MAX && SysTick_Cnt - key_tingle_timer < KEY_TIMER_3S_MAX) //去抖动时间到，产生键值 
		{	
			if(	 key_status_isdown() )
			{	
				if(key_status == KEY_VALUE_NONE)
				{
					key_value = key_value_temp; 
					key_status = key_value_temp;
				}
			}	
			else
			{
				if(key_status < KEY_VALUE_STATUS_DOWN)
					key_value = (KEY_VALUE_T)(key_value_temp + (KEY_VALUE_UP1 - KEY_VALUE_DOWN1));
				key_value_temp = KEY_VALUE_NONE;
				key_status = KEY_VALUE_NONE;
			}
		}
		else if(SysTick_Cnt - key_tingle_timer > KEY_TIMER_3S_MAX) 
		{	
			if(	 key_status_isdown() )
			{	
					key_value = (KEY_VALUE_T)(key_value_temp + (KEY_VALUE_DOWN1_3S - KEY_VALUE_NONE)); 
					key_status = key_value_temp;
			}	
			else
			{
				if(key_status < KEY_VALUE_STATUS_DOWN)
					key_value = (KEY_VALUE_T)(key_value_temp + (KEY_VALUE_UP1 - KEY_VALUE_DOWN1));
				key_value_temp = KEY_VALUE_NONE;
				key_status = KEY_VALUE_NONE;
			}			
		}
		else
		{
			uint8_t value = key_status_isdown() ;
			if(	 value == 0 || value !=  key_value_temp)
			{
				key_value_temp = KEY_VALUE_NONE;
				key_status = KEY_VALUE_NONE;
			}
		}
	}

}

//获取键值
KEY_VALUE_T get_key_value(void)
{
	KEY_VALUE_T key_value_temp;
	key_value_temp = key_value;
	key_value = KEY_VALUE_NONE;
	return key_value_temp;
}




