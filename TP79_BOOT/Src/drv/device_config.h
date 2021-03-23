#ifndef __DEVICE_CONFIG_H_
#define __DEVICE_CONFIG_H_


/*   for device config  */

//�豸�Ƿ��Key
#define DEVICE_KEY    		1

//�豸�Ƿ��LED ָʾ
#define DEVICE_LED   			1

//�豸�Ƿ��poc
#define DEVICE_POC   			1

//uart1
#define DEVICE_GPS  1


#define DEVICE_TIM3  			0

#define DEVICE_ADC   			1

//lcd 8080�ӿ�
#define DEVICE_LCD  		  1

//�����ֿ� ,SPI�ӿ�
#define SPI_GT21L 		1

//����
#define DEVICE_FRIEND		1

#define DEVICE_CALL_LOG		1


/*   for device opt  */

#if  DEVICE_POC
#define DEVICE_POC_UART2
#endif

#if DEVICE_GPS
#define DEVICE_GPS_UART1  
#endif

#if DEVICE_LCD
#define LCD_DEVICE_NV3021
#endif




#endif
