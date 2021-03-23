#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"

#include "adc.h"
#include "key.h"

uint16_t ADCresults[ADC_CHANNELS][ADC_SAMPLE_COUNT];//AD�ɼ�ֵ
uint32_t AD_Data[ADC_CHANNELS];      //AD�ɼ�ֵ
__IO uint16_t ADC_ConvertedValue[ADC_SAMPLE_COUNT][ADC_CHANNELS];



Adc_t Adc;

		
static  void key_gpio_init(GPIO_TypeDef* Port,uint16_t Pin)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Port, &GPIO_InitStructure);	
}


/**
  * @brief  DMA ������ʽ����
  * @param  ��
  * @retval ��
  */
static void ADC_DMA_Init(void)
{
	DMA_InitTypeDef   DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	/* ʹ��DMAʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

	/* ��λDMA1_Channel1 */
	DMA_DeInit(DMA1_Channel1);
	
	/*DMA����ADC����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	
	/*DMA�ڴ����ַ*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	
	/*������Ϊ���ݴ���Դ*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	/*DMAͨ����DMA����Ĵ�С*/
	DMA_InitStructure.DMA_BufferSize = ADC_CHANNELS * ADC_SAMPLE_COUNT;
	
	/*�����ַ�Ĵ�������*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	/*�ڴ��ַ�Ĵ�������*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	/*����ȡֵ��С����Ϊ����*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	/*���ݴ�С����Ϊ����*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	/*DMAѭ��ģʽ������ɺ����¿�ʼ����*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	
	/*DMA���ȼ�����Ϊ��*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	/*DMAͨ��x�����ڴ浽�ڴ洫��*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*DMA��ʼ��*/
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/*���һ��DMA�жϱ�־*/
	DMA_ClearITPendingBit(DMA1_IT_TC1);                                 
	
	/*ʹ��DMA��������ж�*/
	DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE); 
	
	/*ѡ��DMA1ͨ���ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  

	/*�ж�ʹ��*/ 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	
	/*���ȼ���Ϊ0 */
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;  
	
	/*ʹ�� DMA �ж�*/ 
	NVIC_Init(&NVIC_InitStructure);  
 
	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
	 


void ADC_init(void)
{
	
	ADC_InitTypeDef     ADC_InitStructure;

	key_gpio_init(KEY_ADC1_PORT,KEY_ADC1_PIN);
	key_gpio_init(KEY_ADC2_PORT,KEY_ADC2_PIN);	
	
	/*����ADC��Ƶ����Ϊ4 ,ADC���ʱ�䲻�ܳ���14M*/
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);	

	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC1 DeInit */  
	ADC_DeInit(ADC1);
	
	/*��ʼ��ADC�ṹ�壬�˾����ӣ����ӵĻ���·ADC���ݻύ��*/
	ADC_StructInit(&ADC_InitStructure);                         

	/*����ADC�ֱ���Ϊ12λ*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;

	/*��������ת��*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 

	/*��ֹ������⣬ʹ���������*/
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

	/*ADC�ɼ������Ҷ���*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

	/*����ɨ��*/
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;

	/*ADC��ʼ��*/
	ADC_Init(ADC1, &ADC_InitStructure); 
                              
	/*���ò���ͨ����ʱ��*/
	//ADC_ChannelConfig(ADC1, ADC_Channel_1 | ADC_Channel_7, ADC_SampleTime_239_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_7 , ADC_SampleTime_239_5Cycles);

	/* ADC У׼ */
	ADC_GetCalibrationFactor(ADC1);

	/* ѭ��ģʽ�µ� ADC DMA ���� */
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	/* ʹ�� ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);  

	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);     

	/* �ȴ� ADCEN ��־ */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

	/*  ADC1 �����������ת�� */  
	ADC_StartOfConversion(ADC1);
		
	ADC_DMA_Init();	
}

/**
  * @brief  ADC�˲�
  * @param  ��
  * @retval ��
  */
static uint8_t ADC_Filter(void)
{
//	static uint32_t Test_cnt=0;
	uint8_t i, j;
	if(Adc.Flag==1)
	{
		Adc.Flag=0;
		
		/*��DMA������ȡ��AD����*/
		for(i=0; i<ADC_CHANNELS; i++)
		{
			for(j=0; j<ADC_SAMPLE_COUNT; j++)
			{
				ADCresults[i][j] = ADC_ConvertedValue[j][i];	
			}	
		}
		/*ȡֵ���ȡƽ��*/
		for(i=0; i<ADC_CHANNELS; i++)
		{
			AD_Data[i] = 0;
			for(j=0; j<ADC_SAMPLE_COUNT; j++)
			{
				AD_Data[i] += ADCresults[i][j];
			}		
			Adc.Value[i] = AD_Data[i] / ADC_SAMPLE_COUNT;
		}
		return 1;
	}
	return 0;
//	Printf("\r\n AD_value1=%d  AD_value2=%d \r\n",AD_Data[0], AD_Data[1]); 
}


uint16_t get_adc_value(uint8_t adc_chanel)
{
	while(0 == ADC_Filter());
	return Adc.Value[adc_chanel];
}

