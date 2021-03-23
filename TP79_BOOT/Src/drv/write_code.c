#include "stm32f10x.h"
#include "write_code.h"
#include "stm32f10x_gpio.h"
#include "uart.h"
#include "string.h"
#include "my_string.h"
#include "device.h"
#include "poc.h"
#include "friend.h"
#include "call_log.h"

extern device_uart_t  device_uart3;
write_code_status_t write_code_status;


void write_code_init(void)//初始化读写gpio  
{
	write_code_status = WRITE_CODE_NOMAL;
	uart3_config();
}
write_code_status_t write_code_get_status(void)
{
	return write_code_status;
}
void write_code_uart_msg_deal(uint8_t *msg, uint16_t msglen)
{
	uint16_t i,j,k;
	if(memcmp(msg, "AT+POC_TEST", 11) == 0)
	{
		device_cfg_pt cfg = device_config_read();	
		uint8_t ver[32] = "POC_SOF_VER=VVK-S5-R11-170720\r\n";
		memcpy(&ver[12],cfg->ver,17);
		uart3_send_string(ver,31);
	}

	else if(memcmp(msg, "POC_FAC_RESET:\r\n", 16) == 0)
	{
		device_config_set_default();
		friend_deinit();
		call_log_deinit();
		uart3_send_string((uint8_t* )"POC_FAC_RESET:<OK>\r\n",20); 
		return;		
	}
	//发送所有配置 POC_INQUIRE_ALL:<SUCCESS>[card,card]（ip=114.141.132.55;id=JQTEST15;pwd=y1d2w3l4） {OR0,INV1}\r\n
	else if(memcmp(msg, "POC_INQUIRE_ALL:<", 17) == 0)
	{
		uint8_t msg_str[512] = {0};
		uint16_t len;
		device_cfg_pt cfg = device_config_read();	
		for(i=17;i<msglen; i++)
		{
			if(msg[i] == '>')
			{
				if(cfg->dealers_pwd[0] == 0xff)//没有设置过经销商密码
				{
					if(memcmp("000000",&msg[17],i-17) != 0)//设置的密码不是000000
					{
						memset(cfg->dealers_pwd,0xff,33);
						memcpy(cfg->dealers_pwd,&msg[17] ,i-17);//存储经销商密码
					}
				}
				else//比对经销商密码
				{
					if(memcmp(cfg->dealers_pwd,&msg[17],i-17) != 0 && 
						memcmp(PWD_SUPER_USER,&msg[17],strlen(PWD_SUPER_USER)) != 0)
					{
						uart3_send_string((uint8_t* )"POC_INQUIRE_ALL:<PWD_ERROR>\r\n",27); 
						return;
					}
				}
			}
		}		
		
		write_code_status = WRITE_CODE_READ_CFG;
		memcpy(msg_str,"POC_INQUIRE_ALL:<SUCCESS>[",26);
		len = 26;
		memcpy(&msg_str[len],cfg->apn_name, strlen((char*)cfg->apn_name));
		len += strlen((char*)cfg->apn_name);
		memcpy(&msg_str[len],"," ,1);
		len += 1;
		memcpy(&msg_str[len],cfg->apn_pwd, strlen((char*)cfg->apn_pwd));
		len += strlen((char*)cfg->apn_pwd);	
		memcpy(&msg_str[len],"](69703d" ,8);//](ip=
		len += 8;
		hex_to_str(&msg_str[len],cfg->server_ip,strlen((char*)cfg->server_ip));
		len += strlen((char*)cfg->server_ip)*2;
		memcpy(&msg_str[len],"3b69643d" ,8);//;id=
		len += 8;
		hex_to_str(&msg_str[len],cfg->user_id,strlen((char*)cfg->user_id));
		len += strlen((char*)cfg->user_id)*2;
		memcpy(&msg_str[len],"3b7077643d" ,10);//;pwd=
		len += 10;
		hex_to_str(&msg_str[len],cfg->user_pwd,strlen((char*)cfg->user_pwd));
		len += strlen((char*)cfg->user_pwd)*2;
		memcpy(&msg_str[len],"3b00){OR0,INV1}\r\n" ,17);
		memcpy(&msg_str[len+8],&cfg->offline_sound_flag,1);
		memcpy(&msg_str[len+13],&cfg->single_call_flag,1);
		uart3_send_string(msg_str,strlen((char*)msg_str));
	}
	//POC_CFG:<000000>[card,card](69703d3131342e3134312e3133322e35353b69643d4a515445535431353b7077643d7931643277336c343b00)
	else if(memcmp(msg, "POC_CFG:<", 9) == 0)//设置
	{
		device_cfg_pt cfg = device_config_read();	
		for(i=9;i<msglen; i++)
		{
			if(msg[i] == '>')
			{
				if(cfg->dealers_pwd[0] == 0xff)//没有设置过经销商密码
				{
					if(memcmp("000000",&msg[9],i-9) != 0)//设置的密码不是000000
					{
						memset(cfg->dealers_pwd,0xff,33);
						memcpy(cfg->dealers_pwd,&msg[9] ,i-9);//存储经销商密码
						device_config_save_flash();
					}
				}
				else//比对经销商密码
				{
					if(memcmp(cfg->dealers_pwd,&msg[9],i-9) != 0 && 
						memcmp(PWD_SUPER_USER,&msg[9],strlen(PWD_SUPER_USER)) != 0)//密码错误
					{
						uart3_send_string((uint8_t* )"USR_CFG_ERROR_PWD\r\n",19); 
						return;
					}
					else//密码正确
					{
						if(memcmp((&msg[i+1]),"POC_CLR_PWD",11) == 0)//清除密码
						{
								memset(cfg->dealers_pwd,0xff,33);
								device_config_save_flash();			
								uart3_send_string((uint8_t* )"POC_CFG_DEV_PWD_CLEAR_OK\r\n",26); 
								return;
						}
					}
				}
			}
		}
		
		write_code_status = WRITE_CODE_WRITE_CFG;
		for(i=0; i<msglen; i++)
		{
			if(msg[i] == '[')//APN id  pwd
			{
				for(j=i;j<msglen;j++)
				{
					if(msg[j] == ']')
					{
						for(k=i;k<j;k++)
						{
							if(msg[k] == ',')
							{
								memset(cfg->apn_name, 0,33);
								memset(cfg->apn_pwd, 0,33);
								memcpy(cfg->apn_name, &msg[i+1],(k-i-1)%33);
								memcpy(cfg->apn_pwd, &msg[k+1],(j-k-1)%33);
								poc_write_apn(cfg->apn_name, cfg->apn_pwd );
							}
						}
					}
				}
			}
			
			if(msg[i] == '(')//ip, name, pwd
			{
				for(j=i; j<msglen; j++)
				{
					if(msg[j] == ')')
					{
					  uint8_t msg_hex[256] = {0};
						uint8_t k_temp;
						str_to_hex(&msg[i+1],msg_hex,j-i);
						for(k=0;k<256;k++)//ip
						{
							if(msg_hex[k] == ';')
							{
								memset(cfg->server_ip, 0,17);
								memcpy(cfg->server_ip, &msg_hex[3],(k-3)%17);
								k_temp = k+1;
								break;
							}
						}
						for(k = k_temp;k<256;k++)//id
						{
							if(msg_hex[k] == ';')
							{
								memset(cfg->user_id, 0,33);
								memcpy(cfg->user_id, &msg_hex[k_temp+3],(k-k_temp - 3)%33);
								k_temp = k+1;
								break;								
							}
						}		

						for(k = k_temp;k<256;k++)//pwd
						{
							if(msg_hex[k] == ';')
							{
								memset(cfg->user_pwd, 0,33);
								memcpy(cfg->user_pwd, &msg_hex[k_temp+4],(k-k_temp - 4)%33);
								k_temp = k+1;
								break;								
							}
						}						
					}
				}
				
				device_config_save_flash();
				poc_write_code(cfg->server_ip, cfg->user_id,cfg->user_pwd);
				uart3_send_string((uint8_t* )"USR_CFG_SUCCES\r\n",16); 
			}

			if(msg[i] == '{')//气他参数 {OR0,INV1}
			{
				if(msg[i+3] == '0' || msg[i+3] == '1' )
					cfg->offline_sound_flag = msg[i+3];
				if(msg[i+9] == '0' || msg[i+9] == '1' )
					cfg->single_call_flag = msg[i+9];		
				device_config_save_flash();		
			}			
			
		}
	}
}





void write_code_task(void)
{

	if(device_uart3.status == UART_STATUS_RECVED)//有数据未处理
	{
		write_code_uart_msg_deal(device_uart3.recv_buf,device_uart3.write_p);
		device_uart3.status  = UART_STATUS_NORMAL;
	}	
	
}


