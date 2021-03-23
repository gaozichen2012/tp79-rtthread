/**--------------------------------------------------------------------------------------------------------
** Created date:        2017-08-22
** Version:             V1.00
** Descriptions:     s5����Ӧ��   �����л� 

�����������̣�
1   ����ɫ��
2   ui ��ʾ����Log����ʾ��ص���
3   ���Poc״̬����ʾ��ӭʹ�öԽ�ϵͳ�� 
4   ������������ �� ��ɫ����˸
5   ui ��ʾ��������....
6   ��ʾ�ѵ�¼xxx  ����ʾ�����棬��ʾʱ��


�޿�������
�����������̣�
1   ����ɫ��
2   ui ��ʾ����Log�� ��ʾ��ص���
3   ���Poc״̬����ʾ��ӭʹ�öԽ�ϵͳ�� 
4   �������磬��ɫ�Ʋ���˸
5   ��ʾ�����ߣ� ��忨



ptt�������½��뽲�����ɿ��˳���   ��Ⱥ�����Ա����״̬�£� ptt������ȷ��  

�����ϼ���  �̰�����Ⱥ��ѡ�񣬳��������Աѡ��
��Ⱥ�����Ա������ѡ��ҳ�棬 Ϊ�Ϸ��˵�

�����¼���  �̰�����Ⱥ�飬 �����������
��Ⱥ�����Ա������ѡ��ҳ�棬 Ϊ�·��˵�


----------------------------------------------------------------------------------------------------------**/

#include "stm32f10x.h"
#include "stdio.h"
#include "device_config.h"
#include "application.h"
#include "led.h"
#include "poc.h"
#include "nv3021.h"
#include "key.h"
#include "string.h"
#include "my_string.h"
#include "friend.h"
#include "stdlib.h"
#include "gps.h"
#include "device.h"
#include "call_log.h"
#include "stm32f10x_rtc.h"
#include "adc.h"
#include "gps.h"
#include "rtc_drv.h"
#include "stm32f10x_gpio.h"
#include "write_code.h"
#include "flash.h"

#define LED_BLE  LED1
#define LED_RED  LED2


app_device_t  app_device ;
extern uint32_t SysTick_Cnt ;

static void app_menu_set_gourp(KEY_VALUE_T key_value);
static void app_menu_set_member(KEY_VALUE_T key_value);
static void app_menu_set_friend(KEY_VALUE_T key_value);
static void app_menu_set_log(KEY_VALUE_T key_value);
static void app_menu_set_gps(KEY_VALUE_T key_value);
static void app_menu_set_setup(KEY_VALUE_T key_value);
void (*app_fun_set_sunmenu1[6])(KEY_VALUE_T)= {app_menu_set_gourp,app_menu_set_member,app_menu_set_friend,
																						app_menu_set_log,app_menu_set_gps,app_menu_set_setup};

uint16_t application_get_menu_select_last(void)
{
	return app_device.menu_last_select;	
}
app_status_t application_get_status(void)
{
	return app_device.status ;
}

uint16_t application_get_menu_select(void)
{
	return app_device.menu_select;
}


static uint8_t app_keyvalue_to_num(KEY_VALUE_T key_value)//���ּ��� ��ֵת���� num1  = key1 ,num2 = key2 .....
{
	uint8_t key_value_num = 0xff;
	if(key_value >= KEY_FUN_NUM1_DOWN && key_value <= KEY_FUN_NUM3_DOWN)
		key_value_num = key_value - KEY_FUN_NUM1_DOWN +1;
	else if(key_value >= KEY_FUN_NUM4_DOWN && key_value <= KEY_FUN_NUM6_DOWN)
		key_value_num = key_value - KEY_FUN_NUM4_DOWN +4;
	else if(key_value == KEY_FUN_NUM0_DOWN)
		key_value_num = key_value - KEY_FUN_NUM0_DOWN +0;
	else if(key_value >= KEY_FUN_NUM7_DOWN && key_value <= KEY_FUN_NUM9_DOWN)
		key_value_num = key_value - KEY_FUN_NUM7_DOWN +7;	
	
	return key_value_num;
	
}

void application_init(void)
{
	memset((uint8_t*)&app_device,0,sizeof(app_device_t));
}
//add_sub  > 0  ��  ���� ��
void application_select_change(uint8_t add_sub, uint16_t cnt, uint16_t sum)
{
	if(add_sub)
	{		
		app_device.menu_select = (app_device.menu_select+cnt)%sum ;			
	}
	else
	{
		if(app_device.menu_select >= cnt)app_device.menu_select -= cnt;
		else app_device.menu_select = sum - (cnt-app_device.menu_select);
	}
}

void application_state_change(app_status_t state)
{
	app_device.status = state;
	app_device.change_time = SysTick_Cnt;
	app_device.menu_last_select = app_device.menu_select;
	app_device.menu_select = 0;
}

uint32_t app_state_get_change_timer()
{
	return SysTick_Cnt - app_device.change_time;
}

static void application_play_tts_group(uint16_t group)//����Ⱥ�� ����
{
	uint8_t temp[64] = {0},i;	
	poc_device_pt poc_para = poc_get_para();
	if(group == 0)//������ǰȺ��
	{
		 for(i=0;i<50;i++)
		 {
			 if(memcmp(poc_para->group_number_cur, poc_para->group_list_id[i], 4) == 0)
				 break;
		 }		 
		 hex_to_str(temp,poc_para->group_list[i],POC_NAME_LENTH_MAX);
		 poc_audio_tts_paly(temp); 			
	}
	else
	{
		hex_to_str(temp,poc_para->group_list[group],POC_NAME_LENTH_MAX);
		poc_audio_tts_paly(temp);
	}
}


//����״̬�µİ�����������
static void app_working_locked(	KEY_VALUE_T key_value)
{
	poc_device_pt poc_para = poc_get_para();
	
	if(key_value == KEY_FUN_JING_DOWN)  //#�� ������Ļ��ص���Ļ
		{LCD_BK_REVERSE();}	
	else if(key_value == KEY_FUN_LEFT_DOWN)//���������Ļ���뿪ʼ����״̬  
		application_state_change(APP_STATUS_WORKING_UNLOCKING);
	else if(key_value == KEY_FUN_LOW_UP )//�ɿ���������Ⱥ��
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
			application_play_tts_group(0);	
		else
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");
			
	}
	
	else if(key_value == KEY_FUN_LOW_DOWN_3S)//����3��������ѡ��
	{
		poc_audio_tts_paly((uint8_t*)"7D59CB530990E96200"); 
		application_state_change( APP_STATUS_SET_SUBMENU1_3);
	}	
	
	else if(key_value == KEY_FUN_HIGHT_DOWN_3S )//����3������Աѡ��
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
		{
			poc_audio_tts_paly((uint8_t*)"106258540990E96200"); 
			poc_member_refresh();				
			application_state_change(APP_STATUS_SET_SUBMENU1_2_SEACHING);
		}
	}	
	else if(key_value == KEY_FUN_HIGHT_UP )//Ⱥ��ѡ���ݼ����ɿ��ϼ�
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
		{
			 poc_audio_tts_paly((uint8_t *)"A47FC47E0990E96200");
			 application_state_change(APP_STATUS_SET_SUBMENU1_1);			
		}
		else
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");

	}	
}



//�����˵��µ� ������������
static void app_working_main(KEY_VALUE_T key_value)
{
	poc_device_pt poc_para = poc_get_para();
	if(key_value == KEY_FUN_JING_DOWN)//#���� ����
	{
		LCD_BK_OFF();		
		application_state_change(APP_STATUS_WORKING_LOCKED);
	}		
	else if(key_value == KEY_FUN_HIGHT_DOWN_3S )//����3������Աѡ��
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
		{
			poc_audio_tts_paly((uint8_t*)"106258540990E96200"); 
			poc_member_refresh();				
			application_state_change(APP_STATUS_SET_SUBMENU1_2_SEACHING);
		}
		else
		{
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");			
		}
	}	
	
	else if(key_value == KEY_FUN_LEFT_DOWN)//���� ��������˵�
		application_state_change( APP_STATUS_MENU_SET_MAIN);
	else if(key_value == KEY_FUN_LOW_UP )//�ɿ���������Ⱥ��
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
			 application_play_tts_group(0);
		else
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");
	}		

	else if(key_value == KEY_FUN_LOW_DOWN_3S)//����3��������ѡ��
	{
		poc_audio_tts_paly((uint8_t*)"7D59CB530990E96200"); 
		application_state_change(APP_STATUS_SET_SUBMENU1_3);
	}
		
	else if(key_value == KEY_FUN_HIGHT_UP )//Ⱥ��ѡ���ݼ����ɿ��ϼ�
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
		{
			 poc_audio_tts_paly((uint8_t *)"A47FC47E0990E96200");
			 application_state_change(APP_STATUS_SET_SUBMENU1_1);			
		}
		else
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");
	}
}

//�����水����������
static void app_menu_main(KEY_VALUE_T key_value)
{
	poc_device_pt poc_para = poc_get_para();
	if(key_value == KEY_FUN_LEFT_DOWN)  //����������˵�
		application_state_change(APP_STATUS_WORKING_MAIN);
	else if(key_value == KEY_FUN_UP_DOWN)//�ϼ��л� ���˵�ѡ�� �����6��ͼ��
		application_select_change(0,1,6);
	else if(key_value == KEY_FUN_DOWN_DOWN )//�¼��л� ���˵�ѡ�� �����6��ͼ��
		application_select_change(1,1,6);	
	else if(key_value == KEY_FUN_LOW_DOWN_3S)//����3��������ѡ��
	{
		poc_audio_tts_paly((uint8_t*)"7D59CB530990E96200"); 
		application_state_change(APP_STATUS_SET_SUBMENU1_3);
	}
	else if(key_value == KEY_FUN_HIGHT_DOWN_3S )//����3������Աѡ��
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
		{
			poc_audio_tts_paly((uint8_t*)"106258540990E96200"); 
			poc_member_refresh();				
			application_state_change(APP_STATUS_SET_SUBMENU1_2_SEACHING);
		}
		else
		{
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");			
		}
	}	
	
	else if(key_value == KEY_FUN_HIGHT_UP )//Ⱥ��ѡ���ݼ����ɿ��ϼ�
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
		{
			 poc_audio_tts_paly((uint8_t *)"A47FC47E0990E96200");
			 application_state_change(APP_STATUS_SET_SUBMENU1_1);			
		}
		else
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");
	}						
	
	else if(key_value == KEY_FUN_JING_DOWN)//#���� ����
	{
		poc_audio_pa_en(0);
		LCD_BK_OFF();		
		application_state_change(APP_STATUS_WORKING_LOCKED);
	}		

	else if(key_value == KEY_FUN_RIGHT_DOWN || key_value == KEY_FUN_OK_DOWN )//�Ҽ���OK��ȷ��ѡ��
	{
		if(application_get_menu_select() ==  0 ) //����Ⱥ��ѡ��
		{
			if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
			{
				poc_audio_tts_paly((uint8_t *)"A47FC47E0990E96200");
				poc_send_cmd(ATCMD_POC_REFRESH_GROUP);//ˢ��Ⱥ��	
				application_state_change(APP_STATUS_SET_SUBMENU1_1);
			}
			else
				poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");					
		}
		else if(application_get_menu_select() == 1)//�����Աѡ��
		{
			if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
			{
				poc_audio_tts_paly((uint8_t*)"106258540990E96200"); 	
				poc_member_refresh();//ˢ�³�Ա
				application_state_change(APP_STATUS_SET_SUBMENU1_2_SEACHING);
			}
			else
				poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");			
		}		
		else if(application_get_menu_select() == 3)//������¼
		{
			application_state_change(APP_STATUS_SET_SUBMENU1_4);
		}		
		else if(application_get_menu_select() == 2)//�������ѡ��
		{   
			poc_audio_tts_paly((uint8_t*)"7D59CB530990E96200"); 
			application_state_change(APP_STATUS_SET_SUBMENU1_3);
		}
		else if(application_get_menu_select() == 4)//GPS
		{
			poc_audio_tts_paly((uint8_t*)"47005000530000");
			application_state_change(APP_STATUS_SET_SUBMENU1_5);
		}			
		else if(application_get_menu_select() == 5)
		{
			application_state_change(APP_STATUS_SET_SUBMENU1_6);
		}		
					
	}	
}
//Ⱥ�����ð�����������
static void app_menu_set_gourp(KEY_VALUE_T key_value)
{
	poc_device_pt poc_para = poc_get_para();
	uint8_t key_value_num = app_keyvalue_to_num(key_value);
	if(key_value == KEY_FUN_UP_DOWN || key_value == KEY_FUN_HIGHT_DOWN)  //�ϼ��ƶ��˵�������Ⱥ������
	{
		application_select_change(0,1,poc_para->group_list_sum);
		application_play_tts_group(application_get_menu_select()+1);
	}
	else if(key_value == KEY_FUN_DOWN_DOWN || key_value == KEY_FUN_LOW_DOWN)//�¼��ƶ��˵�������Ⱥ������
	{
		application_select_change(1,1,poc_para->group_list_sum);
		application_play_tts_group(application_get_menu_select()+1);
	}
	else if(key_value == KEY_FUN_LEFT_DOWN )  //����˳�Ⱥ��ѡ��
	{
		poc_audio_tts_paly((uint8_t *)"0090FA5100");
		application_state_change( APP_STATUS_MENU_SET_MAIN);
	}		
	else if(key_value == KEY_FUN_RIGHT_DOWN || key_value == KEY_FUN_OK_DOWN ||key_value == KEY_FUN_PTT_UP)//�Ҽ���Ptt����OK������ȷ�Ͻ���Ⱥ�飬֮�󷵻�������
	{
		poc_group_set_new(application_get_menu_select() + 1);
		//����������ϵͳ������ǿ�ƴ�PA 
		poc_audio_pa_en_timer(4000);
		application_state_change( APP_STATUS_WORKING_MAIN); 
	}		
	else if(key_value == KEY_FUN_JING_DOWN)//��#����  �·�10��Ⱥ��
	{
		application_select_change(1,10,poc_para->group_list_sum);
		application_play_tts_group(application_get_menu_select()+1);
	}				
	else if(key_value_num>0 && key_value_num < KEY_VALUE_STATUS_DOWN)//�����ּ�1~9 ֱ��ѡ��Ⱥ��
	{
		if( key_value_num <= poc_para->group_list_sum )
		{
			app_device.menu_select = key_value_num-1;
			application_play_tts_group(application_get_menu_select()+1);
		}					
	}	
}

//��Ա���ð�����������
static void app_menu_set_member(KEY_VALUE_T key_value)
{
	uint8_t temp[64] = {0};
	uint8_t key_value_num = app_keyvalue_to_num(key_value);
	poc_device_pt poc_para = poc_get_para();
	device_cfg_pt cfg = device_config_read();

	if(key_value == KEY_FUN_LEFT_DOWN ) //����������˵�
	{
		poc_audio_tts_paly((uint8_t *)"0090FA5100");
		application_state_change(APP_STATUS_MENU_SET_MAIN);
	}	
	if(poc_para->member_online_cnt >0)
	{
		if(key_value == KEY_FUN_PTT_UP  && poc_para->member_online_cnt > 0 && cfg->single_call_flag == '1')//ptt�� ֱ�Ӻ��г�Ա
		{
			uint8_t call_name[64] = "7C54EB53";
			if(memcmp(poc_para->member_list_id[application_get_menu_select()+1],poc_para->user_id,4) == 0)//���ܺ����Լ�
				poc_audio_tts_paly((uint8_t *)"0D4EFD807C54EB53EA81F15D00");	
			else 
			{
				poc_call_member(application_get_menu_select()+1);
	#if DEVICE_CALL_LOG
	{
				RTC_TimeTypeDef cur_time;
				RTC_GetTime(RTC_Format_BCD, &cur_time);	
				call_log_add(cur_time,poc_para->call_name,poc_para->call_id,1);	
	}	
	#endif							
				hex_to_str(&call_name[8],poc_para->call_name,POC_NAME_LENTH_MAX);
				poc_audio_tts_paly(call_name);
				application_state_change( APP_STATUS_CALLING );		
			}						
		}
		else if(key_value == KEY_FUN_RIGHT_DOWN )  //�Ҽ�������Ա
		{
			application_state_change( APP_STATUS_SET_SUBMENU1_2_1 );	
		}				
		
		else if(key_value == KEY_FUN_UP_DOWN || key_value == KEY_FUN_HIGHT_DOWN)  //�ϼ��ƶ��˵���������Ա����
		{
			memset(temp,0,64);
			application_select_change(0,1,poc_para->member_online_cnt);	
			hex_to_str(temp,poc_para->member_list[application_get_menu_select()+1],POC_NAME_LENTH_MAX);
			poc_audio_tts_paly(temp); 
		}
		else if(key_value == KEY_FUN_DOWN_DOWN || key_value == KEY_FUN_LOW_DOWN)//�¼��ƶ��˵���������Ա����
		{
			memset(temp,0,64);
			application_select_change(1,1,poc_para->member_online_cnt);	
			hex_to_str(temp,poc_para->member_list[application_get_menu_select()+1],POC_NAME_LENTH_MAX);
			poc_audio_tts_paly(temp);
		}	
		else if(key_value == KEY_FUN_JING_DOWN)//��#����  �·�10����Ա
		{
			if(application_get_menu_select()+10 < (poc_para->member_online_cnt-1))
			{
				memset(temp,0,64);
				application_select_change(1,10,poc_para->member_online_cnt);	
				hex_to_str(temp,poc_para->member_list[application_get_menu_select()+1],POC_NAME_LENTH_MAX);
				poc_audio_tts_paly(temp);				
			}
		}
		
		else if(key_value_num>0 && key_value_num < KEY_VALUE_STATUS_DOWN)//�����ּ�1~9 ֱ��ѡ���Ա
		{
			if(key_value_num <= poc_para->member_online_cnt  )
			{		
				memset(temp,0,64);	
				app_device.menu_select = key_value_num - 1;
				hex_to_str(temp,poc_para->member_list[application_get_menu_select()+1],POC_NAME_LENTH_MAX);
				poc_audio_tts_paly(temp);			
			}						
		}	
	}
}

//�������ð�����������
static void app_menu_set_friend(KEY_VALUE_T key_value)
{

	uint8_t temp[64] = {0};
	uint8_t key_value_num ; 
	poc_device_pt poc_para = poc_get_para();
	friend_t fd = {0};
	device_cfg_pt cfg = device_config_read();

	if(key_value == KEY_FUN_LEFT_DOWN ) //����˳�
	{
		application_state_change( APP_STATUS_MENU_SET_MAIN );
		poc_audio_tts_paly((uint8_t *)"0090FA5100");
	}	
	else if(key_value == KEY_FUN_PTT_UP && friend_read_cnt()>0 && cfg->single_call_flag == '1')//ptt�� ֱ�Ӻ��к���
	{
			uint8_t call_name[64] = "7C54EB53";
			friend_t fd;
			friend_read(application_get_menu_select(),&fd);
			poc_call_friend(fd.friend_id,fd.friend_name);
#if DEVICE_CALL_LOG
{
				RTC_TimeTypeDef cur_time;
				RTC_GetTime(RTC_Format_BCD, &cur_time);	
				call_log_add(cur_time,poc_para->call_name,poc_para->call_id,1);	
}			
#endif
			//�������������� xxx
			hex_to_str(&call_name[8],poc_para->call_name,POC_NAME_LENTH_MAX);
			poc_audio_tts_paly(call_name);
			application_state_change( APP_STATUS_CALLING );	
		
	}	

	else if(key_value == KEY_FUN_RIGHT_DOWN &&  friend_read_cnt()>0)  //������Ѳ���
	{
		application_state_change( APP_STATUS_SET_SUBMENU1_3_1 );  
	}	

	else if(key_value == KEY_FUN_UP_DOWN || key_value == KEY_FUN_HIGHT_DOWN)  //�ϼ��ƶ��˵�
	{
		application_select_change(0, 1, friend_read_cnt() );					
		friend_read(application_get_menu_select(),&fd);
		hex_to_str(temp,fd.friend_name,POC_NAME_LENTH_MAX);
		poc_audio_tts_paly(temp); 
	}
	else if(key_value == KEY_FUN_DOWN_DOWN || key_value == KEY_FUN_LOW_DOWN)//�¼��ƶ��˵�
	{
		application_select_change(1, 1, friend_read_cnt());					
		friend_read(application_get_menu_select(),&fd);
		hex_to_str(temp,fd.friend_name,POC_NAME_LENTH_MAX);
		poc_audio_tts_paly(temp); 
	}	
	else if(key_value == KEY_FUN_JING_DOWN)//��#����  �·�10����Ա
	{
		if(application_get_menu_select()+10 < (friend_read_cnt()-1))
		{
			app_device.menu_select  += 10;
			friend_read(application_get_menu_select(),&fd);
			hex_to_str(temp,fd.friend_name,POC_NAME_LENTH_MAX);
			poc_audio_tts_paly(temp); 			
		}
	}
	
	else if(key_value_num>0 && key_value_num < KEY_VALUE_STATUS_DOWN)//�����ּ�1~9 ֱ��ѡ���Ա
	{
		if(key_value_num <= friend_read_cnt()  )
		{				
			app_device.menu_select = key_value_num - 1;
			friend_read(application_get_menu_select(),&fd);
			hex_to_str(temp,fd.friend_name,POC_NAME_LENTH_MAX);
			poc_audio_tts_paly(temp); 		
		}						
	}
}

//��־���ã�������������
static void app_menu_set_log(KEY_VALUE_T key_value)
{
		if(key_value == KEY_FUN_LEFT_DOWN )//�������
		{
			poc_audio_tts_paly((uint8_t *)"0090FA5100");
			application_state_change( APP_STATUS_MENU_SET_MAIN );				
		}	
		if(key_value == KEY_FUN_UP_DOWN)  //�ϼ��ƶ�ѡ��
			application_select_change(0, 1, call_log_read_cnt() );				 
		else if(key_value == KEY_FUN_DOWN_DOWN )  //�¼��ƶ�ѡ��
			application_select_change(1, 1, call_log_read_cnt() );			
		else if(key_value == KEY_FUN_JING_DOWN)//#�� �ƶ�10��ѡ��
		{
			if(application_get_menu_select()+10 < call_log_read_cnt())
				app_device.menu_select  += 10;
		}		
		else if(key_value == KEY_FUN_RIGHT_DOWN && call_log_read_cnt() >0 )  //�ʼ�ȷ�ϣ�������־����
			application_state_change( APP_STATUS_SET_SUBMENU1_4_1 );			
}

//GPS���ã�������������
static void app_menu_set_gps(KEY_VALUE_T key_value)
{
	if(key_value == KEY_FUN_LEFT_DOWN )
	{
		poc_audio_tts_paly((uint8_t *)"0090FA5100");
		application_state_change( APP_STATUS_MENU_SET_MAIN );
	}	
}

//setup���ã�������������
static void app_menu_set_setup(KEY_VALUE_T key_value)
{
	if(key_value == KEY_FUN_UP_DOWN)  //�ϼ��ƶ�ѡ��
		application_select_change(0, 1, 5);					 

	else if(key_value == KEY_FUN_DOWN_DOWN )  //�¼��ƶ�ѡ��
		application_select_change(1, 1, 5 );	

	else if(key_value == KEY_FUN_LEFT_DOWN )//����������˵�
		application_state_change(APP_STATUS_MENU_SET_MAIN);

	else if(key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN)//ok��
	{	
		if(application_get_menu_select()== 0)  //������Ļ
			application_state_change (APP_STATUS_SET_SUBMENU1_6_1);
		else if(application_get_menu_select() == 1)//��ʾ����汾
			application_state_change (APP_STATUS_SET_SUBMENU1_6_2);
		else if(application_get_menu_select() == 2)//�ָ�����
			application_state_change(APP_STATUS_SET_SUBMENU1_6_3);
		else if(application_get_menu_select() == 3)//PPT������
			application_state_change(APP_STATUS_SET_SUBMENU1_6_4);		
		else if(application_get_menu_select() == 4)//������
			application_state_change(APP_STATUS_SET_SUBMENU1_6_5);				
	}	
}


	
//Ⱥ�齲����PTT����
static void app_ptt_talk_task(KEY_VALUE_T key_value)
{
	static uint8_t talk_flag = 0;
	static uint8_t ptt_stop_flag = 0;
	static uint32_t key_down_timer;
	static uint8_t timer_over_flag = 1;	
	poc_device_pt poc_para = poc_get_para();
	device_cfg_pt cfg = device_config_read();
	if(poc_para->talker_id[0] != 0 )//Ⱥ�����˽�������������Ļ����ap
	{
		if(talk_flag == 0)
		{
			talk_flag = 1;
			poc_audio_pa_en(1);
			LCD_BK_ON();
			LED_blink(LED_BLE,0,0);//ֹͣ��ɫLED��˸����LED
			LED_ON(LED_BLE);
			if(ptt_stop_flag == 0 && timer_over_flag == 0)		//�����ptt�Խ�״̬�������������˳�ptt����
			{
				ptt_stop_flag = 1;
				key_down_timer = SysTick_Cnt;		
			}
		}
	}	
	else//�˳�����
	{
		if(talk_flag)
		{
			talk_flag = 0;
			poc_audio_pa_en(0);
			if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
				LED_blink(LED_BLE,500,3000);
		}				
	}

	if( key_value ==  KEY_FUN_PTT_DOWN )//��⵽����ptt
	{
		if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)//poc����
		{
			if(application_get_status() != APP_STATUS_SET_SUBMENU1_1 &&
				application_get_status() != APP_STATUS_SET_SUBMENU1_2 &&
				application_get_status() != APP_STATUS_SET_SUBMENU1_3	&&
				application_get_status() > APP_STATUS_SEACH_NETWORK )//����POC����
			{
				if(poc_para->talker_priority || poc_para->talker_id[0] == 0)//���˽������߿�������
				{
					ptt_stop_flag = 0;
					timer_over_flag = 0;
					if(poc_para->talker_id[0] != 0)//���ԭ���ڽ�����ǿ���˳�����
					{
						poc_talker_exit();
					}
					if(application_get_status() != APP_STATUS_BE_CALLED && application_get_status() != APP_STATUS_CALLING)
						application_state_change( APP_STATUS_WORKING_MAIN );
					LCD_BK_ON();		
					LED_ON(LED_RED);	
					LED_blink(LED_BLE,0,0);
					poc_ptt_talk_start();				
					key_down_timer = SysTick_Cnt;		
					
					if(cfg->ptt_sound)//�����
						poc_audio_pa_en_timer(300);			
				}
				else
				{
					poc_ptt_talk_start_fail_sound();
					ptt_stop_flag = 1;
				}
			}
		}
		else
		{
			if(application_get_status() > APP_STATUS_SEACH_NETWORK )//����POC����			
			poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");//������
		}
	}
	else if(key_value ==  KEY_FUN_PTT_UP && ptt_stop_flag == 0 && timer_over_flag == 0)//�ɿ�������400ms���������
	{
		ptt_stop_flag = 1;
		timer_over_flag = 1;
		key_down_timer = SysTick_Cnt;			
	}
	if(ptt_stop_flag  && (SysTick_Cnt - key_down_timer > 400) ) //PTT�ɿ�400ms ��������
	{
			ptt_stop_flag = 0;
		  timer_over_flag = 1;
			LED_OFF(LED_RED);
			if(poc_para->talker_id[0] == 0 &&	poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
				LED_blink(LED_BLE,500,3000);
			poc_ptt_talk_stop();
			//�����
			if(cfg->ptt_sound && poc_para->talker_id[0] == 0)			
				poc_audio_pa_en_timer(300);						
	}		
	if(SysTick_Cnt - key_down_timer > 50000 && timer_over_flag == 0)//��������50��
	{
		timer_over_flag = 1;
		ptt_stop_flag = 1;
		key_down_timer = SysTick_Cnt;		
	}

}
	
static void app_bat_check_task()
{
  uint16_t BatLevelValue[13]={2850,2900,3080,3210,3260,3350,3470,3570,3670,3790,3880,4100,5000};//,2800�ػ�������5000����Ϊ��ӵ�ֵ
	static uint16_t cur_level;
  uint16_t  adc_value;
	uint8_t i;
	adc_value = get_adc_value(0);	
	for(i=0; i<13; i++)
	{
		if(adc_value < BatLevelValue[i])
			break;
	}
	cur_level = i;
	
	if(1)
	{
		static uint32_t play_timer = 0xffffffff,times = 0;

		if(cur_level <= 1  && application_get_status() != APP_STATUS_DEVICE_SHUTDOWN && application_get_status() != DEVICE_STATUS_SLEEP )
		{
			if(times++ > 10)
			{
				poc_audio_tts_paly((uint8_t *)"73513A6700");
				LCD_BK_ON();
				poc_shut_down();//�ص�POCģ��
				application_state_change( APP_STATUS_DEVICE_SHUTDOWN);	
			}
		}
		else if(cur_level <= 2)//�����ͣ�ÿ��10���Ӳ���һ��
		{
			times = 0;
			if(play_timer == 0xffffffff || SysTick_Cnt - play_timer > 600000)
			{
				play_timer = SysTick_Cnt;
				poc_audio_tts_paly((uint8_t *)"3575CF914E4F00");
			}
		}
		else
		{
			times = 0;
		}
	}
	
}

static void app_gps_manger_task()
{
	static uint32_t gps_timer = 0;
	static uint16_t send_times = 0;
	static double old_ew,old_ns, new_ew, new_ns;
	poc_device_pt poc_para = poc_get_para();
	if((gps_get_position_state() == '1' ||gps_get_position_state() == '2') && 
		poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED && ((SysTick_Cnt - gps_timer) > 5000*send_times))
	{
		gps_timer = SysTick_Cnt;
		old_ns = new_ns;
		old_ew = new_ew; 			
		poc_send_gps_dat(gps_get_NS_value(),gps_get_EW_value());
		send_times++;
	}
	new_ns =  atof((char*)gps_get_NS_value());
	new_ew = atof((char*)gps_get_EW_value());			
	if((new_ns>old_ns &&  new_ns - old_ns > 0.000200) || 
			(new_ns<old_ns &&  old_ns - new_ns  > 0.000200) || 
			(new_ew>old_ew &&  new_ew - old_ew > 0.000200) || 
			(new_ew<old_ew &&  old_ew - new_ew  > 0.000200))
	{
		send_times = 1;
	}			
}	

//�豸Ӧ�� task
void application_task(void)
{
	device_cfg_pt cfg = device_config_read();
	poc_device_pt poc_para = poc_get_para();
	KEY_VALUE_T key_value = get_key_value();
	if(key_value> 0 && key_value <= KEY_VALUE_DOWN21 && KEY_FUN_PTT_DOWN != key_value && application_get_status() > APP_STATUS_SEACH_NETWORK &&
	cfg->key_sound)
		poc_audio_paly_keydown();
		
	switch(application_get_status())
	{
/****       ����            ***/			
		case APP_STATUS_NORMAL:	       
		{
			LED_ON(LED_BLE);
			//���PTT����û�а��£����������д��ģʽ
			if(key_value == KEY_FUN_PTT_DOWN)
			{
				write_code_init();
				application_state_change(APP_STATUS_WRITE_CODE);
				
			}
			//���PTT����û�а��£����������д��ģʽ
			else if(key_value == KEY_FUN_HIGHT_DOWN)
			{
				uint32_t boot_flag = 0x12345678;
				Flash_Updata(FLASH_ADDR_BOOT_FLAG,(uint8_t *)&boot_flag,4);
				NVIC_SystemReset();	
				
			}			
			
			else 
			{
				if(app_state_get_change_timer() > 1000)//1s�����APP_STATUS_STARTING poc  check
					application_state_change(APP_STATUS_STARTING);	
			}
		}break;
		
		case APP_STATUS_WRITE_CODE:	    //д��ģʽ   
		{
			LED_ON(LED_RED);
			LED_blink(LED_BLE,0,0);
			LED_OFF(LED_BLE);
			poc_audio_pa_en(0);
			write_code_task();
		}break;		
		
		
		case APP_STATUS_STARTING: 
		{
			if(poc_para->status == POC_DEVICE_STATUS_CHECKED_OK)
			{
				LED_blink(LED_BLE,500,500);
				poc_audio_tts_paly((uint8_t *)"226BCE8F7F4F2875F95BB28BFB7CDF7E0CFF2C001C64227D517FDC7E");//���Ż�ӭʹ�öԽ�ϵͳ����������
				application_state_change(APP_STATUS_ENTER_POC);  //����ENTER POC״̬
			}	
		}break;
		
/****       ����pocӦ�ã��ȴ�POC��¼OK         ***/	
		case APP_STATUS_ENTER_POC: 
		{	
			if(poc_para->status  == POC_DEVICE_STATUS_LAND_SUCCESSED  )//��⵽POC��¼�ɹ�����ȡ��Ⱥ��Id���û���¼id�����빤��������
			{
				LED_blink(LED_BLE,500,3000);
				poc_audio_pa_en(1);//��ϵͳ���������е�¼�ɹ�����
				application_state_change(APP_STATUS_WORKING_MAIN) ; //����������
			}		
			else if(poc_para->status  == POC_DEVICE_STATUS_SIM_ERROR)
			{
				LED_blink(LED_RED,200,200);
				poc_audio_tts_paly((uint8_t *)"6153025F385E0CFFF78BC068E56700");	
				application_state_change(APP_STATUS_ENTER_SIM_ERROR);  //����ENTER POC״̬				
			}			
		}break;
	
/****  ����״̬     ***/	
		case APP_STATUS_WORKING_LOCKED:
		{
			if(key_value)
				app_working_locked(key_value);
		}break;	
/****  �����У� ��Ҫ�ٰ�* ȷ�Ͻ���    ***/	
		case APP_STATUS_WORKING_UNLOCKING:
		{
			if(key_value == KEY_FUN_JING_DOWN)  //#�� ������Ļ��ص���Ļ
			{
				LCD_BK_REVERSE();			
			}
			else if(key_value == KEY_FUN_XING_DOWN)	//* �� �����ɹ������������湤��״̬
				application_state_change(APP_STATUS_WORKING_MAIN);
			
			if(app_state_get_change_timer() > 5000) //5�����ް������½�������
				application_state_change(APP_STATUS_WORKING_LOCKED);
		}break;
	
/****   �����湤��״̬ ***/	
		case APP_STATUS_WORKING_MAIN: 
		{
			if(key_value)
				app_working_main(key_value);
		}break;	
	
/****   ���ò˵��� ������    ***/	
		case APP_STATUS_MENU_SET_MAIN:
		{
			if(key_value)
				app_menu_main(key_value);
		}break;		

/****   ���˵�-> 6��1���Ӳ˵�����Ĵ�������  ***/	
		case APP_STATUS_SET_SUBMENU1_1:
		case APP_STATUS_SET_SUBMENU1_2: 
		case APP_STATUS_SET_SUBMENU1_3: 
		case APP_STATUS_SET_SUBMENU1_4: 
		case APP_STATUS_SET_SUBMENU1_5: 
		case APP_STATUS_SET_SUBMENU1_6: 	
		{		
			if(key_value)//������Ӧ�Ĳ˵��°�������
				app_fun_set_sunmenu1[application_get_status()-APP_STATUS_SET_SUBMENU1_1](key_value);	
		}break;		
		
/****   ���ò˵��� ��Ա���ã�������Ա   ***/	
		case APP_STATUS_SET_SUBMENU1_2_SEACHING: 
		{
			if(app_state_get_change_timer() > 6000 )//6���ڲ�ѯʧ�ܣ�����������
				application_state_change( APP_STATUS_MENU_SET_MAIN );

			if(poc_para->member_online_cnt > 0) //��ѯ�ɹ����л����Ӳ˵�2 ��Ա����
			{
				uint8_t len, tts_dat[32] = {0};
				poc_para->member_online_cnt -= 1;
				len = uint16_to_unicode_decstr(tts_dat,poc_para->member_online_cnt );
				if(poc_para->member_online_cnt == 0)
				{
					poc_audio_tts_paly((uint8_t *)"E065BA4E2857BF7E00");	
					application_state_change( APP_STATUS_SET_SUBMENU1_2 );
				}
				else
				{
					memcpy(&tts_dat[len],"BA4E2857BF7E00",14);
					poc_audio_tts_paly(tts_dat);	
					application_state_change( APP_STATUS_SET_SUBMENU1_2 );
				}
			}
		}break;
		
/****   ���ò˵��� �Ӳ˵�6 �� �Ӳ˵�3  �ָ���������  ***/	
		case APP_STATUS_SET_SUBMENU1_6_3:
		{
				if(key_value == KEY_FUN_LEFT_DOWN )
				{
					application_state_change( APP_STATUS_SET_SUBMENU1_6 );
				}	
				else if(key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN)//ok��
				{	
					poc_audio_tts_paly((uint8_t *)"BE8B6E7F10629F5200");
					device_config_set_default();
					friend_deinit();
					call_log_deinit();
					application_state_change( APP_STATUS_WORKING_MAIN );
				}					
		}break;
				
/****   calll  ����״̬   ***/		
    case APP_STATUS_BE_CALLED:		
		case APP_STATUS_CALLING:
		{
			  if(application_get_status() == APP_STATUS_CALLING && 
					app_state_get_change_timer() > 5000 && poc_para->call_status != POC_CALL_SUCCESS)
				{
					poc_audio_tts_paly((uint8_t *)"7C54EB533159258D00");
					application_state_change( APP_STATUS_WORKING_MAIN );
				}
				if(key_value == KEY_FUN_LEFT_DOWN)  //����������˵�
				{
					//ȡ������
					poc_call_clr();
					poc_audio_tts_paly((uint8_t *)"0090FA5100");
					application_state_change( APP_STATUS_WORKING_MAIN );
				}	
				//���Poc״̬�����poc״̬���гɹ�����ʾ�������гɹ�
				if(poc_para->call_status == POC_CALL_SUCCESS)
				{
					//poc_audio_tts_paly((uint8_t *)"7C54EB5310629F5200");	//��POC���沥��				
				}
				//���Poc״̬�����poc״̬����ʧ�ܣ���ʾ����������ʧ��
				else if(poc_para->call_status == POC_CALL_FAILED)
				{
					poc_audio_tts_paly((uint8_t *)"7C54EB533159258D00");
					application_state_change( APP_STATUS_WORKING_MAIN );
				}
				//���poc״̬�����Poc״̬����������������ʾ���˳����������˵�
				else if(poc_para->call_status == POC_CALL_EXIT )
				{
					poc_audio_tts_paly((uint8_t *)"0090FA5100");
					application_state_change( APP_STATUS_WORKING_MAIN );
				}
				//�������30�룬���Զ��˳��������˵�
				if(app_state_get_change_timer()> 30000)
					application_state_change( APP_STATUS_WORKING_MAIN );
				
		}break;
/****   �����Ӳ˵���Ա���ò����˵�    ***/					
		case  APP_STATUS_SET_SUBMENU1_2_1:  
		{
			device_cfg_pt cfg = device_config_read();
			if(key_value == KEY_FUN_LEFT_DOWN ) //������س�Ա���ò˵�
				application_state_change( APP_STATUS_SET_SUBMENU1_2 );
			else if(key_value == KEY_FUN_UP_DOWN)//�ϼ��ƶ�ѡ��
				application_select_change(0, 1, 3);						 
			else if(key_value == KEY_FUN_DOWN_DOWN )//�¼� �ƶ�ѡ��
				application_select_change(1, 1, 3);				
			else if((key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN) && cfg->single_call_flag == '1')//ok��
			{
				if(application_get_menu_select() == 0)//���г�Ա
				{
					uint8_t call_name[64] = "7C54EB53";
					if(memcmp(poc_para->member_list_id[application_get_menu_select_last()+1],poc_para->user_id,4) == 0)//���ܺ����Լ�
					{
						poc_audio_tts_paly((uint8_t *)"0D4EFD807C54EB53EA81F15D00");	
					}
					else
					{
						poc_call_member(application_get_menu_select_last()+1);
						
#if DEVICE_CALL_LOG
{
						RTC_TimeTypeDef cur_time;
						RTC_GetTime(RTC_Format_BCD, &cur_time);	
						call_log_add(cur_time,poc_para->call_name,poc_para->call_id,1);	
}	
#endif							
						//�������������� xxx
						hex_to_str(&call_name[8],poc_para->call_name,POC_NAME_LENTH_MAX);
						poc_audio_tts_paly(call_name);
						application_state_change( APP_STATUS_CALLING );
					}
				}
				else if(application_get_menu_select() == 1)//�Ӻ���
				{
					friend_t fd = {0};
					memcpy(fd.friend_id,poc_para->member_list_id[application_get_menu_select_last()+1],4); 
					memcpy(fd.friend_name,poc_para->member_list[application_get_menu_select_last()+1],POC_NAME_LENTH_MAX); 
				  if(memcmp(poc_para->member_list_id[application_get_menu_select_last()+1],poc_para->user_id,4) != 0)//���ܺ����Լ�
					{
						if( friend_add(fd) == 1)//��ӳɹ�����ʾ��Ӻ��ѳɹ����ظ�����
							poc_audio_tts_paly((uint8_t *)"2959A0527D59CB5310629F5200");	//��Ӻ��ѳɹ�
						else if(friend_add(fd) == 2)
							poc_audio_tts_paly((uint8_t *)"CD910D597D59CB5300");	//�ظ�����
						else
							poc_audio_tts_paly((uint8_t *)"2959A0527D59CB533159258D00");	//���ʧ��
						application_state_change( APP_STATUS_SET_SUBMENU1_2 );
					}
					else
					{
						poc_audio_tts_paly((uint8_t *)"0D4EFD80FB6DA052EA81F15D00");	//��������Լ�
						application_state_change( APP_STATUS_SET_SUBMENU1_2 );
					}
				}
				else if(application_get_menu_select() == 2)//ȡ��
				{
					application_state_change( APP_STATUS_SET_SUBMENU1_2 );
				}
			}
		}break;
		
/****   ������ ���� ***/					
		case  APP_STATUS_SET_SUBMENU1_6_5:  
		{
			if(key_value == KEY_FUN_LEFT_DOWN ) 
				application_state_change( APP_STATUS_SET_SUBMENU1_6 );			
			else if(key_value == KEY_FUN_UP_DOWN)//�ϼ��ƶ�ѡ��
				application_select_change(0, 1, 2);					 
			else if(key_value == KEY_FUN_DOWN_DOWN )//�¼� �ƶ�ѡ��
				application_select_change(1, 1, 2);			
			else if(key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN)//ok��
			{
				device_cfg_pt cfg = device_config_read();
				if(application_get_menu_select() == 0)
				{
					cfg->key_sound = 1;
					device_config_save_flash();
				}
				else if(application_get_menu_select() == 1)
				{
					cfg->key_sound = 0;
					device_config_save_flash();
				}
			
				poc_audio_tts_paly((uint8_t *)"BE8B6E7F10629F5200");
				app_device.status = APP_STATUS_SET_SUBMENU1_6;
			}					
			
		}break;		
		
/****  PTT�� ����***/					
		case  APP_STATUS_SET_SUBMENU1_6_4:  
		{		
			if(key_value == KEY_FUN_LEFT_DOWN ) 
				application_state_change( APP_STATUS_SET_SUBMENU1_6 );	
			else if(key_value == KEY_FUN_UP_DOWN)//�ϼ��ƶ�ѡ��
				application_select_change(0, 1, 2);					 
			else if(key_value == KEY_FUN_DOWN_DOWN )//�¼� �ƶ�ѡ��
				application_select_change(1, 1, 2);			
			else if(key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN)//ok��
			{
				device_cfg_pt cfg = device_config_read();
				if(application_get_menu_select() == 0)
				{
					cfg->ptt_sound = 1;
					device_config_save_flash();
				}
				else if(application_get_menu_select() == 1)
				{
					cfg->ptt_sound = 0;
					device_config_save_flash();
				}
		
				poc_audio_tts_paly((uint8_t *)"BE8B6E7F10629F5200");
				app_device.status = APP_STATUS_SET_SUBMENU1_6;
			}					
		}break;
		
/****   �����Ӳ˵� �汾���� ***/					
		case  APP_STATUS_SET_SUBMENU1_6_2:  
		{		
			if(key_value == KEY_FUN_LEFT_DOWN ) 
				application_state_change( APP_STATUS_SET_SUBMENU1_6 );			
		}break;
/****   �����Ӳ˵� ��Ļ����  ***/					
		case  APP_STATUS_SET_SUBMENU1_6_1:  
		{
			if(key_value == KEY_FUN_LEFT_DOWN ) 
				application_state_change( APP_STATUS_SET_SUBMENU1_6 );
			else if(key_value == KEY_FUN_UP_DOWN)//�ϼ��ƶ�ѡ��
				application_select_change(0, 1, 4);					 
			else if(key_value == KEY_FUN_DOWN_DOWN )//�¼� �ƶ�ѡ��
				application_select_change(1, 1, 4);			
			else if(key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN)//ok��
			{
				device_cfg_pt cfg = device_config_read();			
				if(application_get_menu_select() == 0)
					cfg->lcd_sleep_timer = 30000;
				else if(application_get_menu_select() == 1)
					cfg->lcd_sleep_timer = 60000;
				else if(application_get_menu_select() == 2)
					cfg->lcd_sleep_timer = 0xffffffff;
				else if(application_get_menu_select() == 3)
					cfg->lcd_sleep_timer = 21000;	
				device_config_save_flash();
				poc_audio_tts_paly((uint8_t *)"BE8B6E7F10629F5200");
				app_device.status = APP_STATUS_SET_SUBMENU1_6;
			}		
			
		}break;

		
		

/****   �����Ӳ˵� �������ò����˵�  ***/					
		case  APP_STATUS_SET_SUBMENU1_3_1:  
		{
			device_cfg_pt cfg = device_config_read();
			if(key_value == KEY_FUN_LEFT_DOWN ) 
				application_state_change( APP_STATUS_SET_SUBMENU1_3 ); 
			else if(key_value == KEY_FUN_UP_DOWN)//�ϼ��ƶ�ѡ��
				application_select_change(0, 1, 3);						 

			else if(key_value == KEY_FUN_DOWN_DOWN )//�¼� �ƶ�ѡ��
				application_select_change(1, 1, 3);		
	
			else if((key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN) && cfg->single_call_flag == '1')//ok��
			{
				if(application_get_menu_select() == 0)//���к���
				{
					if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
					{					
						uint8_t call_name[64] = "7C54EB53";
						friend_t fd;
						friend_read(application_get_menu_select_last(),&fd);
						poc_call_friend(fd.friend_id,fd.friend_name);
	#if DEVICE_CALL_LOG
	{
							RTC_TimeTypeDef cur_time;
							RTC_GetTime(RTC_Format_BCD, &cur_time);	
							call_log_add(cur_time,poc_para->call_name,poc_para->call_id,1);	
	}			
	#endif
						//�������������� xxx
						hex_to_str(&call_name[8],poc_para->call_name,POC_NAME_LENTH_MAX);
						poc_audio_tts_paly(call_name);
						application_state_change( APP_STATUS_CALLING );	
					}
					else
					{
						poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");
						application_state_change( APP_STATUS_SET_SUBMENU1_3 );	
					}

				}
				else if(application_get_menu_select() == 1)//ɾ��
				{
					if(friend_del(application_get_menu_select_last()))
						poc_audio_tts_paly((uint8_t *)"2052649610629F5200");	
					else
						poc_audio_tts_paly((uint8_t *)"205264963159258D00");	

					application_state_change( APP_STATUS_SET_SUBMENU1_3 ); 
				}
				else if(application_get_menu_select() == 2)//ȡ��
				{
					application_state_change( APP_STATUS_SET_SUBMENU1_3 );
				}
			}
		}break;		
		
/****   �����Ӳ˵� ��¼���ò����˵�  ***/				
		case  APP_STATUS_SET_SUBMENU1_4_1:  
		{
			device_cfg_pt cfg = device_config_read();
			if(key_value == KEY_FUN_LEFT_DOWN ) 
				application_state_change( APP_STATUS_SET_SUBMENU1_4 );
			else if(key_value == KEY_FUN_UP_DOWN)//�ϼ��ƶ�ѡ��
				application_select_change(0, 1, 3);						 
			else if(key_value == KEY_FUN_DOWN_DOWN )//�¼� �ƶ�ѡ��
				application_select_change(1, 1, 3);		
		
			else if((key_value == KEY_FUN_OK_DOWN || key_value == KEY_FUN_RIGHT_DOWN )&& cfg->single_call_flag == '1')//ok��
			{
				if(application_get_menu_select() == 0)//���к���
				{
					if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED)
					{
						uint8_t call_name[64] = "7C54EB53";
						call_log_t call_log;
						call_log_read(application_get_menu_select_last()+1,&call_log);
						poc_call_friend(call_log.caller_id,call_log.caller_name);
	#if DEVICE_CALL_LOG
	{
							RTC_TimeTypeDef cur_time;
							RTC_GetTime(RTC_Format_BCD, &cur_time);	
							call_log_add(cur_time,poc_para->call_name,poc_para->call_id,1);	
	}			
	#endif
						//�������������� xxx
						hex_to_str(&call_name[8],poc_para->call_name,POC_NAME_LENTH_MAX);
						poc_audio_tts_paly(call_name);
						application_state_change( APP_STATUS_CALLING );	
					}
					else
					{
						poc_audio_tts_paly((uint8_t *)"0D4E2857BF7E00");
						application_state_change( APP_STATUS_SET_SUBMENU1_4 );	
					}
				}
				else if(application_get_menu_select() == 1)//ɾ��
				{
					call_log_del(application_get_menu_select_last());
					poc_audio_tts_paly((uint8_t *)"2052649610629F5200");
					application_state_change( APP_STATUS_SET_SUBMENU1_4 );						
				}
				else if(application_get_menu_select()== 2)//ȡ��
				{
					application_state_change( APP_STATUS_SET_SUBMENU1_4 );
				}
			}
		}break;				

		
		//�ػ�״̬ �ػ���
		case APP_STATUS_DEVICE_SHUTDOWN:
		{
			//���ADCֵ�������0����ر�POWER�� ����ǵ�ص�ѹ�͹ػ�������
			uint16_t  adc_value;
			adc_value = get_adc_value(0);	
			//�ȵ�POC�����ͨ�˾͹ػ�
			if(adc_value < 1000)
			{
				uint32_t delay = 30000000;
				while(delay--);
				//nv3020_color_clr(0,0,COLOR_BLACK,160,128);
				nv3021_display_turnoff();
				nv3021_sleep_in();
				POC_POWER_OFF();
			}
			else  if(adc_value > 1000)//��Ƭ������˯��״̬
			{
				uint32_t delay = 30000000;
				while(delay--);
				nv3021_display_turnoff();
				nv3021_sleep_in();
				//GPSʡ��ģʽ
				GPS_mode_set_power_save();
				poc_audio_pa_en(0);	
				LED_blink(LED_RED,0,0);		
				LED_blink(LED_BLE,0,0);		
				LCD_REST_ON();		
				LCD_BK_OFF();	
				USART_DeInit(USART1);
				USART_DeInit(USART2);
				rtc_sleepmode_init();	
				SysTick->CTRL  = 0;
				//����˯��ģʽ		
				while(1)
				{	
					static uint8_t cnt=0;		
					SysTick_Config(48000);
					ADC_init();	
					if(get_adc_value(0)>3400)
						cnt++;
					else if(get_adc_value(0)<1000)
						POC_POWER_OFF();
					else
						cnt = 0;
					if(cnt>10)
						NVIC_SystemReset();	
					PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
					for(delay = 0; delay<100000; delay++);
					PWR_ClearFlag(PWR_FLAG_WU);					
				}
			}				
				

		}break;
		
		default :
			break;
			
	}
	
	/****   ����״̬�¶�ִ�е� Ӧ��  ***/	
	//gps����״̬�ϱ�
	app_gps_manger_task();
	//��Դ����
	app_bat_check_task();
	//ptt����
	app_ptt_talk_task(key_value);

	//����� ������Ļ
	if(key_value &&  key_value < KEY_VALUE_STATUS_DOWN && 
		application_get_status() != APP_STATUS_DEVICE_SHUTDOWN  && application_get_status() != DEVICE_STATUS_SLEEP)
	{
		if(key_value != KEY_FUN_JING_DOWN)		
		{
			LCD_BK_ON();		
			app_device.change_time = SysTick_Cnt;
		}
	}
	
	//��������
	if(app_state_get_change_timer() > 20000 && 
		app_device.status > APP_STATUS_WORKING_MAIN  &&
	  (poc_para->status == POC_CALL_NOMAL || poc_para->status == POC_CALL_EXIT) &&
       poc_para->talker_id[0] == 0	)
	{
		application_state_change( APP_STATUS_WORKING_MAIN );
	}		

	//�Զ�����  
	if(app_state_get_change_timer() > cfg->lcd_sleep_timer  && 
		app_device.status >= APP_STATUS_WORKING_LOCKED  &&
	  (poc_para->call_status == POC_CALL_NOMAL || poc_para->call_status == POC_CALL_EXIT) &&
        poc_para->talker_id[0] == 0   )
	{
		LCD_BK_OFF();
		application_state_change( APP_STATUS_WORKING_LOCKED );
	}		
	
	//home������
	if(application_get_status() >= APP_STATUS_MENU_SET_MAIN)
	{
		if(key_value == KEY_FUN_HOME_DOWN) //HOME	
			application_state_change( APP_STATUS_WORKING_MAIN );
	}	
	
	//����
	if(APP_STATUS_CALLING != application_get_status() && 
		 APP_STATUS_BE_CALLED != application_get_status() )
	{
		device_cfg_pt cfg = device_config_read();
		if(poc_para->call_status == POC_BE_CALLED && cfg->single_call_flag == '1')
		{
		//	RTC_TimeTypeDef cur_time;
			application_state_change( APP_STATUS_BE_CALLED ); 
			//��¼����Log
		//	RTC_GetTime(RTC_Format_BCD, &cur_time);	
		//	call_log_add(cur_time,poc_get_call_name(),poc_get_call_id(),0);
			uint8_t call_name[64] = {0};
			LCD_BK_ON();			
			LED_ON(LED_BLE);			
			hex_to_str(call_name,poc_para->call_name,POC_NAME_LENTH_MAX);
			memcpy(&call_name[strlen((char*)call_name)],"7C54EB53116200",14);
			poc_audio_tts_paly(call_name);
		}
	}
	
}







