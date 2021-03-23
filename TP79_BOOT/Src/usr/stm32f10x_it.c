/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_dma.h"

#include "stdlib.h"
#include "string.h"

#include "uart.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint64_t SysTick_Cnt;
extern device_uart_t   device_uart2;



/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
//extern volatile GUI_TIMER_TIME OS_TimeMS;
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	SysTick_Cnt++;

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void TIM6_IRQHandler(void)
{
	static uint64_t counter = 0;
	///OSIntEnter();
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

	///SysTick_Cnt++;
	///U8SqStaus = SQ_PinStatusCheck();

	
	////ButtonLeftSide_TimerCount();

	///ToneSoundPinOutput();
	///OSIntExit();
}


/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
	  	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			if(((device_uart2.write_p+1)%UART_RXBUF_LEN_MAX) != device_uart2.read_p)
			{
				device_uart2.recv_buf[device_uart2.write_p++] = (uint8_t)USART_ReceiveData(USART2);	
				
			}
			else//¶ª°üÁË
			{
				USART_ReceiveData(USART2);	
				// LED_ON(1);
			}
			device_uart2.write_p %= UART_RXBUF_LEN_MAX;
			device_uart2.status = UART_STATUS_RECVING;
			USART_ClearFlag(USART2,USART_FLAG_RXNE);
	}
//	if(USART_GetITStatus(USART2, USART_IT_RTO) != RESET) 
	//{
	//	USART_ClearITPendingBit(USART2, USART_IT_RTO);	
//	}
	USART_ClearFlag(USART2,0xFFFFFFFF);
	

}



/*******************************************************************************
* Function Name  : DMAChannel1_IRQHandler
* Description    : This function handles DMA Stream 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void DMA1_Channel1_IRQHandler(void)
{

	
}







/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
