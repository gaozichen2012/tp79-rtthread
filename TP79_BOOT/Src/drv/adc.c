#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"

#include "adc.h"
#include "key.h"

uint16_t ADCresults[ADC_CHANNELS][ADC_SAMPLE_COUNT];//AD采集值
uint32_t AD_Data[ADC_CHANNELS];      //AD采集值
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
  * @brief  DMA 工作方式配置
  * @param  无
  * @retval 无
  */
static void ADC_DMA_Init(void)
{
	DMA_InitTypeDef   DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	/* 使能DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

	/* 复位DMA1_Channel1 */
	DMA_DeInit(DMA1_Channel1);
	
	/*DMA外设ADC基地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	
	/*DMA内存基地址*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	
	/*外设作为数据传输源*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	/*DMA通道的DMA缓存的大小*/
	DMA_InitStructure.DMA_BufferSize = ADC_CHANNELS * ADC_SAMPLE_COUNT;
	
	/*外设地址寄存器不变*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	/*内存地址寄存器递增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	/*外设取值大小设置为半字*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	/*数据大小设置为半字*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	/*DMA循环模式，即完成后重新开始覆盖*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	
	/*DMA优先级设置为高*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	/*DMA通道x禁用内存到内存传输*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*DMA初始化*/
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/*清除一次DMA中断标志*/
	DMA_ClearITPendingBit(DMA1_IT_TC1);                                 
	
	/*使能DMA传输完成中断*/
	DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE); 
	
	/*选择DMA1通道中断*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  

	/*中断使能*/ 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	
	/*优先级设为0 */
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;  
	
	/*使能 DMA 中断*/ 
	NVIC_Init(&NVIC_InitStructure);  
 
	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
	 


void ADC_init(void)
{
	
	ADC_InitTypeDef     ADC_InitStructure;

	key_gpio_init(KEY_ADC1_PORT,KEY_ADC1_PIN);
	key_gpio_init(KEY_ADC2_PORT,KEY_ADC2_PIN);	
	
	/*设置ADC分频因子为4 ,ADC最大时间不能超过14M*/
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);	

	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC1 DeInit */  
	ADC_DeInit(ADC1);
	
	/*初始化ADC结构体，此句必须加，不加的话多路ADC数据会交换*/
	ADC_StructInit(&ADC_InitStructure);                         

	/*配置ADC分辨率为12位*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;

	/*开启连续转换*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 

	/*禁止触发检测，使用软件触发*/
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

	/*ADC采集数据右对齐*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

	/*向上扫描*/
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;

	/*ADC初始化*/
	ADC_Init(ADC1, &ADC_InitStructure); 
                              
	/*配置采样通道及时间*/
	//ADC_ChannelConfig(ADC1, ADC_Channel_1 | ADC_Channel_7, ADC_SampleTime_239_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, ADC_Channel_7 , ADC_SampleTime_239_5Cycles);

	/* ADC 校准 */
	ADC_GetCalibrationFactor(ADC1);

	/* 循环模式下的 ADC DMA 请求 */
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	/* 使能 ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);  

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);     

	/* 等待 ADCEN 标志 */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

	/*  ADC1 常规软件启动转换 */  
	ADC_StartOfConversion(ADC1);
		
	ADC_DMA_Init();	
}

/**
  * @brief  ADC滤波
  * @param  无
  * @retval 无
  */
static uint8_t ADC_Filter(void)
{
//	static uint32_t Test_cnt=0;
	uint8_t i, j;
	if(Adc.Flag==1)
	{
		Adc.Flag=0;
		
		/*从DMA缓存中取出AD数据*/
		for(i=0; i<ADC_CHANNELS; i++)
		{
			for(j=0; j<ADC_SAMPLE_COUNT; j++)
			{
				ADCresults[i][j] = ADC_ConvertedValue[j][i];	
			}	
		}
		/*取值求和取平均*/
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

