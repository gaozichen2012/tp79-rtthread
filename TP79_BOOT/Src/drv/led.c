#include "stm32f10x.h"
#include "led.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#ifdef LED_PIN_F0F1
uint16_t led_pin[LED_MAX] = {LED1_PIN,LED2_PIN};
GPIO_TypeDef* led_port[LED_MAX] = {LED1_PORT,LED2_PORT};
#endif

led_device_t   led_device[LED_MAX];
extern uint32_t SysTick_Cnt ;

static void led_gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i;
	for(i=0; i<LED_MAX; i++)
	{
#ifdef LED_PIN_F0F1	
		RCC_AHBPeriphClockCmd( RCC_AHBENR_GPIOFEN, ENABLE); 	
		GPIO_InitStructure.GPIO_Pin = led_pin[i] ;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(led_port[i], &GPIO_InitStructure);		  
		GPIO_ResetBits(led_port[i], led_pin[i] );	 
#endif		
	}
}

static void led_para_config()
{
	uint8_t i;
	for(i=0; i<LED_MAX; i++)
	{
		led_device[i].LEDn = (led_list_t)i;
		led_device[i].led_blink_en = 0;
		led_device[i].led_blink_timer1 = 0;
		led_device[i].led_blink_timer2 = 0;
	}	
}

void LED_init(void)
{
	led_gpio_config();
	led_para_config();
}

void LED_ON(led_list_t LEDn)
{
	GPIO_SetBits(led_port[LEDn],led_pin[LEDn]);
}
uint8_t LED_READ(led_list_t LEDn)
{
	return GPIO_ReadOutputDataBit(led_port[LEDn],led_pin[LEDn]);
}
void LED_OFF(led_list_t LEDn)
{
	GPIO_ResetBits(led_port[LEDn],led_pin[LEDn]);
}

void LED_REVERSAL(led_list_t LEDn)
{
	if(GPIO_ReadOutputDataBit(led_port[LEDn],led_pin[LEDn]))
		GPIO_ResetBits(led_port[LEDn],led_pin[LEDn]);
	else
		GPIO_SetBits(led_port[LEDn],led_pin[LEDn]);
}

void LED_blink(led_list_t LEDn, uint16_t timeron,uint16_t timeroff)
{
	if(timeron)
	{
		led_device[LEDn].led_blink_en = 1;
		led_device[LEDn].led_blink_timer1 = timeron;
		led_device[LEDn].led_blink_timer2 = timeroff;
	}
	else
	{
		LED_OFF(LEDn);
		led_device[LEDn].led_blink_en = 0;
	}
}

void LED_task(void)
{
	uint8_t i;
	//blink
	for(i=0; i<LED_MAX; i++)
	{
		if(led_device[i].led_blink_en)
		{
			static uint32_t blink_timer ;
			if( LED_READ((led_list_t)i) && SysTick_Cnt - blink_timer >  led_device[i].led_blink_timer1)
			{
				blink_timer = SysTick_Cnt;
				LED_REVERSAL((led_list_t)i);
			}		
			else if(LED_READ((led_list_t)i) == 0 && SysTick_Cnt - blink_timer >  led_device[i].led_blink_timer2)
			{
				blink_timer = SysTick_Cnt;
				LED_REVERSAL((led_list_t)i);
			}				
		}
	}
	
	
}



