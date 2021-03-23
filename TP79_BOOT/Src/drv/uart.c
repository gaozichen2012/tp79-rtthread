#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "device_config.h"
#include "uart.h"
#include "tim3.h"
#include "string.h"

device_uart_t   device_uart2;



/*
 * 函数名：uart2_config
 * 描述    
 * 输入  	：无
 * 输出  	:    无
 */
void uart2_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
   
	/*Enable the USARTx Interrupt(使能USART1中断)*/

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DeInit(USART2);

	/* Enable USART2 clocks */
	///RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	///GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable the USART Receive interrupt: this interrupt is generated when the
	  USART2 receive data register is not empty */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/* Enable the USARTx */
	USART_Cmd(USART2, ENABLE);


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_High;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	/* Configure the USARTx */
	USART_ClockInit(USART2, &USART_ClockInitStructure);
	USART_Init(USART2, &USART_InitStructure);

	device_uart2.read_p = 0;
	device_uart2.write_p = 0;
	memset(device_uart2.recv_buf, 0, UART_RXBUF_LEN_MAX);
	device_uart2.status = UART_STATUS_NORMAL;
	 
}


void uart2_send_byte(uint8_t byte) //发送1字节数据
{
	USART_SendData(USART2, (uint16_t) byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}		

void uart2_send_string(const uint8_t *str, uint16_t strlen)
{
	uint16_t j;
	for(j=0 ;j<strlen ;j++)
	{
		uart2_send_byte(str[j]);
	}

}



