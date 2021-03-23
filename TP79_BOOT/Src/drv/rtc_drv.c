#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include "rtc_drv.h"


void rtc_sleepmode_init(void)
{  
  RTC_TimeTypeDef RTC_TimeStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
  EXTI_InitTypeDef EXTI_InitStructure;
  RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_InitTypeDef   RTC_InitStructure;
	
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

/* LSI used as RTC source clock */
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
		
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
   
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Calendar Configuration */
  RTC_InitStructure.RTC_AsynchPrediv = 99;
  RTC_InitStructure.RTC_SynchPrediv	=  399; /* (40KHz / 100) - 1 = 399*/
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);  

	
  /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Set the alarm X+5s */
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x00;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x01;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x05;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    
  /* Enable the alarm */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
  
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x00;
  RTC_TimeStructure.RTC_Minutes = 0x00;
  RTC_TimeStructure.RTC_Seconds = 0x00;  
  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

}


void Rtc_Init(void)
{
	
  RTC_TimeTypeDef RTC_TimeStructure;
  NVIC_InitTypeDef NVIC_InitStructure; 
  EXTI_InitTypeDef EXTI_InitStructure;
 // RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_InitTypeDef   RTC_InitStructure;
  RTC_DateTypeDef  RTC_DatStructure;	
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

  /* LSI used as RTC source clock */
  /* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
   
  /* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Calendar Configuration */
  RTC_InitStructure.RTC_AsynchPrediv = 99;
  RTC_InitStructure.RTC_SynchPrediv	=  399; /* (40KHz / 100) - 1 = 399*/
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);  

  /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
   /* Set the alarm X+5s */
 // RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
 // RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x00;
 // RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;
 // RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x01;
 // RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
//  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
//  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
//  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

 // RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    
  /* Enable the alarm */
  // RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

  RTC_DatStructure.RTC_Date = 1;
  RTC_DatStructure.RTC_Month = 1;
	RTC_DatStructure.RTC_WeekDay = 1;
	RTC_DatStructure.RTC_Year = 17;
  RTC_SetDate(RTC_Format_BCD, &RTC_DatStructure);
	
	
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x00;
  RTC_TimeStructure.RTC_Minutes = 0x00;
  RTC_TimeStructure.RTC_Seconds = 0x00;  
  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

}



