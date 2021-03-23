#include "tim3.h"




void TIM_INT_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /*  TIM3 �ж�Ƕ�����*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM_OUT_Config(void)
{
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* Time ��ʱ���������� */  //4.16us * 25   9600 104us   
 // TIM_TimeBaseStructure.TIM_Period = 200;
	
	 TIM_TimeBaseStructure.TIM_Period = 20000;  //416us
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Ԥ��Ƶ������ */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* TIM �ж�ʹ�� */
  TIM_ITConfig(TIM3, TIM_IT_CC3 , ENABLE);

  /* TIM3 ʹ�� */
  TIM_Cmd(TIM3, ENABLE);
	
}

void tim3_init(void)
{
	TIM_INT_Config();
	TIM_OUT_Config();	
}

