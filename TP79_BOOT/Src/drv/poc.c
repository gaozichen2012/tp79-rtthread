#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "poc.h"
#include "uart.h"
#include "string.h"
#include "my_string.h"
#include "device.h"
#include "flash.h"
poc_device_t poc_device ;
poc_device_pt poc_device_p;
 
#define START_ADDRESS 0x08004000

extern uint32_t SysTick_Cnt ;

extern device_uart_t  device_uart2;
extern device_cfg_t  device_cfg;
static void delay(uint16_t ms)
{
	uint32_t cnt;
	while(ms--)
	for(cnt=0;cnt<100000;cnt++);
}


static void poc_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  AUDIO_GPIO_POC_RST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(AUDIO_GPIO_POC_RST_PORT, &GPIO_InitStructure);		  
	POC_SW_ON();
	
}


void poc_init(void)
{
	uart2_config();
	poc_gpio_init();
	POC_SW_ON();
	uart2_send_string((const uint8_t*)"AT+CFUN=1,1\r\n", 13);
	delay(20);	
	uart2_send_string((const uint8_t*)"AT+CFUN=1,1\r\n", 13);
	memset((uint8_t *)&poc_device,0,sizeof(poc_device_t));
	poc_device_p  = &poc_device;
}


poc_device_status_t poc_get_status(void)
{
	return poc_device.status;
}

void poc_send_cmd(poc_cmd_t poc_cmd)
{
	uint8_t i;
	const poc_cmd_t cmd[] = {
		
		
		ATCMD_TEST,
		ATCMD_ATE0,
		ATCMD_CPIN,
		ATCMD_POC_OPENING,
		ATCMD_POC_CCLK,
		ATCMD_POC_CSQ,
		ATCMD_POC_REFRESH_GROUP, 
		ATCMD_POWER_OFF,
		ATCMD_POC_CREG,
		ATCMD_ATZADSET,
		ATCMD_POC_MEMBER_SIMPLE_MSG,
		ATCMD_POC_LAND,
		ATCMD_POC_LAND_OUT,
		ATCMD_POWER_RST,
		ATCMD_GET_MEID,
		ATCMD_QIOTRPT,
		ATCMD_POC_REFRESH_GPS,
		ATCMD_POC_PARA_GET
	
	};
	const char poc_cmd_string[][64] = {
		
		POC_CMD_AT,
		POC_CMD_ATE0,
		POC_CMD_CPIN, 
		POC_CMD_POC_OPENING,
		POC_CMD_CCLK, 
		POC_CMD_CSQ, 
		POC_CMD_REFRESH_GROUP,
		AT_CMD_POWER_OFF,
		POC_CMD_CREG ,
		POC_CMD_ZADSET ,
		POC_MEMBER_SIMPLE_MSG,
		POC_CMD_POC_LAND,
		POC_CMD_POC_LAND_OUT,
		AT_CMD_POWER_RST,
		AT_CMD_GET_MEID,
		AT_CMD_QIOTRPT,
		AT_CMD_POC_REFRESH_GPS,
		AT_CMD_POC_PARA_GET
	
	};
	for(i=0; i<sizeof(cmd); i++)
	{
		if(poc_cmd == cmd[i])
			uart2_send_string((const uint8_t*)poc_cmd_string[i], strlen((const char*)poc_cmd_string[i]));			
	}
}




/*******************************************************************************
* Function Name  : calcrc
* Description    : ����CRC.
* Input          : data to calculate
* Output         : crc result
* Return         : None
*******************************************************************************/
unsigned short calcrc(char *ptr, int count)
{
    unsigned short crc = 0;
    char i;

    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
        if (crc & 0x8000)
            crc = crc << 1 ^ 0x1021;
        else
            crc = crc << 1;
        } while(--i);
    }
    return (crc & 0xFFFF);
}


/*
 * ��������poc_uart_msg_deal
 * ����    ����poc��������֡���壬������������
 * ����  ����������֡
 * ���  : 
 */
static void poc_uart_msg_deal(uint8_t *ptr)
{
	if(memcmp(ptr, "OK", 2) == 0) //���յ�Ok ָ��
	{
		if(poc_device.status == POC_DEVICE_STATUS_STARTING) 
		{
			poc_device.status = POC_DEVICE_STATUS_CHECKED_OK; //�豸����ǿ���״̬�����ɼ�⵽POC״̬
		}
		
		else if(poc_device.status == POC_DEVICE_STATUS_SHUTDOWN) 
			poc_device.status = POC_DEVICE_STATUS_SHUTDOWNING; 	
	}
	
//��¼״̬֪ͨ+POC:82 02 000120e2 4b6dd58b310033000000
	else if(memcmp(ptr, "+POC:82", 7) == 0) 
	{
		if(memcmp(ptr+strlen("+POC:82"), "02", 2) == 0)//��¼�ɹ�
		{
			poc_device.status = POC_DEVICE_STATUS_LAND_SUCCESSED;
		}
		else if(memcmp(ptr+strlen("+POC:82"), "01", 2) == 0)//��¼��
		{		
			poc_device.status = POC_DEVICE_STATUS_LANDING;				
		}		
			
		
	}	
	
	
	else if(memcmp(ptr, "+POC:42", 7) == 0) 
	{
		if(memcmp(ptr+14 ,device_cfg.ver,12) == 0)//�Ա�ǰ12λ"TYT_TP79V02_",��ͬ�������һ��
		{
			if(memcmp(ptr+14,device_cfg.ver,strlen((char*)device_cfg.ver)) == 0)//�汾��һ�����˳�����
			{
				poc_device.status = POC_DEVICE_VER_DWONLOAD_SUCCESS; 
				uart2_send_string((const uint8_t*)"AT+QPOWD=0\r\n", 12);
				poc_device.status = POC_DEVICE_VER_SAME;
			}
			else
			{
				poc_device.status = POC_DEVICE_VER_DWONLOAD;
				memset(poc_device.server_soft_ver,0,32);
				memcpy(poc_device.server_soft_ver,ptr+14,(strlen((char*)ptr) -14)%32);	
				memset(&poc_device.server_soft_ver[18],0,32-18);
				memcpy(poc_device.server_soft_id,ptr+9,4);	
			}
		}
	}
	else if(memcmp(ptr, "+POC:4300", 9) == 0) ///ģ����������
	{
		poc_device.down_status = 1;	
		
	}
	else if(memcmp(ptr, "+POC:4302", 9) == 0) //���ص�Ƭ������ģ�����
	{
		uint16_t sum;
		poc_device.status = POC_DEVICE_VER_UPGRADE;	
		str_to_hex(ptr+9,(uint8_t *)&sum ,4);	 
		poc_device.pack_sum = ((sum>>8)&0x00ff) | ((sum<<8)&0xff00);
	}	
	else if(memcmp(ptr, "+POC:4303", 9) == 0) //���ذ�
	{
		uint16_t crc;
		if(strlen((char*)ptr) >= 540)
		{
			uint8_t dat256[256] = {0}, dat256_temp[256];
			uint16_t cnt1,cnt2;
			str_to_hex(&ptr[536],(uint8_t *)&crc,4);
			str_to_hex(&ptr[14],(uint8_t*)&cnt1,4);
			str_to_hex(&ptr[24],dat256,512);
			cnt2 = (((cnt1>>8)&0x00ff) | ((cnt1<<8)&0xff00));
			
			if(crc == calcrc((char*)dat256,256) && cnt2 == poc_device.down_pack_cnt+1)  //У�����ݰ� AND crc
			{
				static uint8_t erase_flag = 0;
				uint32_t addr;
				//����app
				if(erase_flag == 0)
				{
					erase_flag = 1;
					Flash_Erase(START_ADDRESS, 390*1024);
				}			
				addr = (uint32_t)(poc_device.down_pack_cnt*256) + START_ADDRESS;
				Flash_Write(addr, dat256,256);
				Flash_Read_for_download(addr, dat256_temp,256);
				if(memcmp(dat256_temp,dat256,256) == 0)
					poc_device.down_pack_cnt++;	
			}
		}
	}	
	else if(memcmp(ptr, "+POC:4301", 9) == 0) //����
	{

	}		
	
}


void poc_task(void)
{
	static uint32_t resend_timer ;
	if(resend_timer > SysTick_Cnt)
		resend_timer = SysTick_Cnt;	
	
	/***        poc ״̬����             ***/
	switch(poc_device.status)
	{
		//�տ�����״̬������ATE0�ж�ģ����û��init ok
		case POC_DEVICE_STATUS_STARTING:
		{
			if(SysTick_Cnt - resend_timer > 1000)//1�뷢һ��
			{
				resend_timer = SysTick_Cnt;
				poc_send_cmd(ATCMD_ATE0);
			}
		}break;
	
		case POC_DEVICE_STATUS_CHECKED_OK:
		{
			if(SysTick_Cnt - resend_timer > 5000)
			{
				resend_timer = SysTick_Cnt;
				poc_send_cmd(ATCMD_POC_OPENING);
			}					
		}break;
		
		case POC_DEVICE_STATUS_LAND_SUCCESSED:
		{

			if(poc_device.server_soft_ver[0] == 0) //���汾��
			{
				if(SysTick_Cnt - resend_timer > 2000)
				{
					resend_timer = SysTick_Cnt;
					uart2_send_string((const uint8_t*)"AT+POC=420000\r\n", 15);	
				}				
			}			
		}break;
		
		case POC_DEVICE_VER_DWONLOAD://���ع̼���ģ��
		{
			static uint8_t send_times = 0;
			if(poc_device.down_pack_cnt == 0) 
			{
				if((SysTick_Cnt - resend_timer > 5000 || send_times ==0)&&(poc_device.down_status == 0))
				{	
					uint8_t cmd[] = "AT+POC=430000000000\r\n";
					memcpy(&cmd[15],(uint8_t*)&poc_device.server_soft_id,4);
					memcpy(&cmd[19],"\r\n",2);
					resend_timer = SysTick_Cnt;
					uart2_send_string((uint8_t *)cmd, 21);	
					send_times++;
				}				
			}	
		}break;		

		case POC_DEVICE_VER_UPGRADE://��ʼ����
		{
		    static uint16_t pack_cnt;
			static uint16_t old_send_cnt;
			if(poc_device.down_pack_cnt < poc_device.pack_sum)
			{
				if(SysTick_Cnt - resend_timer > 2000 || old_send_cnt != poc_device.down_pack_cnt)
				{
					uint8_t cmd[] = "AT+POC=430000010001\r\n";
					uint8_t cnt[2];
					old_send_cnt = poc_device.down_pack_cnt;
					pack_cnt = poc_device.down_pack_cnt+1;
					//memcpy(&cmd[11],poc_device.server_soft_id,4);
					cnt[1] =  (pack_cnt & 0x00ff);
					cnt[0] =  ((pack_cnt >> 8)&0x00ff);
					hex_to_str(&cmd[15],cnt,2);
					memcpy(&cmd[19],"\r\n",2);
					resend_timer = SysTick_Cnt;
					uart2_send_string(cmd, 21);	
				}	
			}
			else
			{
				poc_device.status = POC_DEVICE_VER_DWONLOAD_SUCCESS; 
				uart2_send_string((const uint8_t*)"AT+QPOWD=0\r\n", 12);
				
				//uart2_send_string((const uint8_t*)"AT+CFUN=1,1\r\n", 13);
			}
				
		}break;
		


		default:
		{
		}break;
	}

	
	/***     poc uart msg deal      ***/
 // do
	{
	  static uint8_t buf_temp_deal[1024]  = {0};
		static uint16_t cmp_cnt = 0;		
		while(device_uart2.read_p != device_uart2.write_p )//������δ����
		{
			if(device_uart2.recv_buf[(device_uart2.read_p+cmp_cnt)%UART_RXBUF_LEN_MAX] == '\r' || 
				 device_uart2.recv_buf[(device_uart2.read_p+cmp_cnt)%UART_RXBUF_LEN_MAX] == '\n')//��⵽δ������������ \r\n��β������
			{	
				if(cmp_cnt >= 2)//ֻ�����ȴ���2��ָ��
				{
					memset(buf_temp_deal,0, 1024);
					if(device_uart2.read_p+cmp_cnt < UART_RXBUF_LEN_MAX)//�������ѭ���洢
						memcpy(buf_temp_deal,&device_uart2.recv_buf[device_uart2.read_p], cmp_cnt);
					else
					{
						memcpy(buf_temp_deal,&device_uart2.recv_buf[device_uart2.read_p], (UART_RXBUF_LEN_MAX - device_uart2.read_p)%1024);
						memcpy(&buf_temp_deal[UART_RXBUF_LEN_MAX - device_uart2.read_p],device_uart2.recv_buf, \
						(device_uart2.read_p+cmp_cnt)%UART_RXBUF_LEN_MAX%(1024 - ((UART_RXBUF_LEN_MAX - device_uart2.read_p)%1024)));
					}
					poc_uart_msg_deal(buf_temp_deal);	
				}
				device_uart2.read_p = (device_uart2.read_p+(cmp_cnt+1))%UART_RXBUF_LEN_MAX ;//�������
				cmp_cnt = 0;
			}
			else
			{
				if( ((device_uart2.read_p < device_uart2.write_p )&& (cmp_cnt+1 < device_uart2.write_p - device_uart2.read_p))||
					  ((device_uart2.read_p > device_uart2.write_p) && ( cmp_cnt+1 < (UART_RXBUF_LEN_MAX - device_uart2.read_p + device_uart2.write_p)) ))
					cmp_cnt++	;
				else
				{
					if(cmp_cnt >= 1024)
					{
						device_uart2.write_p = 0;
						device_uart2.read_p = 0;
						cmp_cnt = 0;
					}						
					break;
				}
			}
		}
	}
	

}



poc_device_pt poc_get_para(void)
{
	return &poc_device;
}


