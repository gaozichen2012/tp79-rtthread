#ifndef __GPS_H_
#define __GPS_H_

#include "stm32f10x.h"
#include "nv3021.h"

#define GPS_REST_OFF()	LCD_REST_OFF()
#define GPS_REST_ON()   LCD_REST_ON()

typedef enum
{
	GPS_DEVICE_NOT_DETECT, //δ��⵽�豸  
  GPS_DEVICE_NORMAL, //����״̬	
  GPS_DEVICE_MOVING, //�ƶ�״̬
	
}gps_device_status_t;

typedef enum
{
	ITUDE_N = 'N',
	ITUDE_S = 'S',
	ITUDE_W = 'W',
	ITUDE_E = 'E',	
	
} itude_t;

typedef struct
{
	uint8_t SNR[32][4];  //����ȣ����32������
	uint32_t time[32];  //����ʱ��,����ʱ�� ��ֵδ���£�������ź�ǿ��Ϊ0
	//uint8_t number[32][4];  //���Ǳ��
	//���ǽǶ�  90��ͷ��
	//��λ�Ƕ�  0~360 ˮƽ
	
}gps_snr_t,*gps_snr_pt;



typedef struct
{
	uint8_t NS_value[32]; //γ�� ֵ
	itude_t ns;        //����
  uint8_t EW_value[32];	//����ֵ
	itude_t ew;    //����
	uint8_t position_state;  //��λ״̬ ��1�� ��λ�ɹ���  ��0�� δ��λ   
	uint8_t satellites_sum[4];  //��������	
  gps_snr_t  snr;   //����״̬��Ϣ  �����
	gps_device_status_t status; 

}gps_device_t,*gps_device_pt;

void gps_init(void);
void gps_task(void);
void GPS_mode_set_power_save(void);


uint8_t* gps_get_NS_value(void) ; //γ��ֵ
itude_t gps_get_NS_type(void) ;  //��γOR��γ
uint8_t* gps_get_EW_value(void); //����ֵ
itude_t gps_get_EW_type(void) ;  //����������
uint8_t gps_get_position_state(void) ; //��λ״̬���Ƿ�λ�ɹ�
uint8_t* gps_get_satellites_sum(void) ; //��������
uint8_t gps_get_status(void) ;  //gps״̬
gps_snr_t* gps_get_snr(void) ; //�ź�״̬

#endif







