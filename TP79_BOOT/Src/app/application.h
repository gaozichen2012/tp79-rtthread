#ifndef __APPLICATION_H_
#define __APPLICATION_H_


#include "poc.h"


#define KEY_FUN_LEFT_DOWN		KEY_VALUE_DOWN1
#define KEY_FUN_LEFT_UP			KEY_VALUE_UP1

#define KEY_FUN_UP_DOWN		KEY_VALUE_DOWN2
#define KEY_FUN_UP_UP			KEY_VALUE_UP2

#define KEY_FUN_RIGHT_DOWN		KEY_VALUE_DOWN3
#define KEY_FUN_RIGHT_UP			KEY_VALUE_UP3
//home ��
#define KEY_FUN_HOME_DOWN		KEY_VALUE_DOWN4
#define KEY_FUN_HOME_UP		KEY_VALUE_UP4

#define KEY_FUN_DOWN_DOWN		KEY_VALUE_DOWN5
#define KEY_FUN_DOWN_UP			KEY_VALUE_UP5

#define KEY_FUN_OK_DOWN		KEY_VALUE_DOWN6
#define KEY_FUN_OK_UP			KEY_VALUE_UP6

#define KEY_FUN_XING_DOWN  KEY_VALUE_DOWN10
#define KEY_FUN_XING_UP  KEY_VALUE_UP10
//# ��
#define KEY_FUN_JING_DOWN		KEY_VALUE_DOWN18
#define KEY_FUN_JING_UP			KEY_VALUE_UP18
//�Խ�ptt��
#define KEY_FUN_PTT_DOWN   KEY_VALUE_DOWN19   
#define KEY_FUN_PTT_UP  	 KEY_VALUE_UP19 

#define KEY_FUN_HIGHT_DOWN   KEY_VALUE_DOWN20  
#define KEY_FUN_HIGHT_UP  	 KEY_VALUE_UP20 
#define KEY_FUN_LOW_DOWN   KEY_VALUE_DOWN21   
#define KEY_FUN_LOW_UP  	 KEY_VALUE_UP21


#define KEY_FUN_HIGHT_DOWN_3S  KEY_VALUE_DOWN20_3S  
#define KEY_FUN_LOW_DOWN_3S   KEY_VALUE_DOWN21_3S   



#define KEY_FUN_NUM1_DOWN 	KEY_VALUE_DOWN7
#define KEY_FUN_NUM1_UP			KEY_VALUE_UP7
#define KEY_FUN_NUM2_DOWN 	KEY_VALUE_DOWN8
#define KEY_FUN_NUM2_UP			KEY_VALUE_UP8
#define KEY_FUN_NUM3_DOWN 	KEY_VALUE_DOWN9
#define KEY_FUN_NUM3_UP			KEY_VALUE_UP9
#define KEY_FUN_NUM4_DOWN 	KEY_VALUE_DOWN11
#define KEY_FUN_NUM4_UP			KEY_VALUE_UP11
#define KEY_FUN_NUM5_DOWN 	KEY_VALUE_DOWN12
#define KEY_FUN_NUM5_UP			KEY_VALUE_UP12
#define KEY_FUN_NUM6_DOWN 	KEY_VALUE_DOWN13
#define KEY_FUN_NUM6_UP			KEY_VALUE_UP13
#define KEY_FUN_NUM0_DOWN 	KEY_VALUE_DOWN14
#define KEY_FUN_NUM0_UP			KEY_VALUE_UP14
#define KEY_FUN_NUM7_DOWN 	KEY_VALUE_DOWN15
#define KEY_FUN_NUM7_UP			KEY_VALUE_UP15
#define KEY_FUN_NUM8_DOWN 	KEY_VALUE_DOWN16
#define KEY_FUN_NUM8_UP			KEY_VALUE_UP16
#define KEY_FUN_NUM9_DOWN 	KEY_VALUE_DOWN17
#define KEY_FUN_NUM9_UP			KEY_VALUE_UP17



typedef enum
{
	APP_STATUS_NORMAL,
	APP_STATUS_DEVICE_SHUTDOWN,  //�ػ�
	DEVICE_STATUS_SLEEP, //˯��
	APP_STATUS_WRITE_PROGRAM,  //д����		
	APP_STATUS_WRITE_CODE,  //д��	
	APP_STATUS_STARTING,  //������
	APP_STATUS_ENTER_POC,  //��⵽POC
	APP_STATUS_ENTER_SIM_ERROR,  //δ�忨
	APP_STATUS_SEACH_NETWORK,  //��������
	APP_STATUS_WORKING_LOCKED,	
	APP_STATUS_WORKING_UNLOCKING, //�����������
	APP_STATUS_WORKING_MAIN,
	
	APP_STATUS_MENU_SET_MAIN,	//�������˵�����
  APP_STATUS_SET_SUBMENU1_1, //һ���Ӳ˵�  Ⱥ��ѡ��
	APP_STATUS_SET_SUBMENU1_2,  //��Աѡ��
	APP_STATUS_SET_SUBMENU1_3,  //����ѡ��
	APP_STATUS_SET_SUBMENU1_4,  //������¼
	APP_STATUS_SET_SUBMENU1_5,  //GPS
	APP_STATUS_SET_SUBMENU1_6,  //����
	
	APP_STATUS_SET_SUBMENU1_2_SEACHING,  //���ڲ�ѯ��Ա�б�
	APP_STATUS_SET_SUBMENU1_2_1,  //�����Ӳ˵���Ա���ò����˵�
	APP_STATUS_SET_SUBMENU1_3_1,  //����ѡ���Ӳ˵�
	APP_STATUS_SET_SUBMENU1_4_1,  //��ʷ���м�¼
	
	APP_STATUS_SET_SUBMENU1_6_1, //�����Ӳ˵�  ������Ļ����
	APP_STATUS_SET_SUBMENU1_6_2, //��ʾ�汾��
	APP_STATUS_SET_SUBMENU1_6_3, //�ָ�����
	APP_STATUS_SET_SUBMENU1_6_4	, //ppt ����
	APP_STATUS_SET_SUBMENU1_6_5, //������
	
	APP_STATUS_CALLING,    //��������״̬
	APP_STATUS_BE_CALLED,    //��������״̬	
	

}app_status_t;



typedef struct
{
	app_status_t status;
  uint16_t menu_select;   //�˵�ѡ��ָ��
  uint16_t menu_last_select;  //�ϼ��˵����л�֮ǰ��ѡ��
	uint32_t change_time;
	
}app_device_t,*app_device_pt;



void application_init(void);
void application_task(void);
app_status_t application_get_status(void);
uint16_t application_get_menu_select(void);	
uint16_t application_get_menu_select_last(void);	



#endif




