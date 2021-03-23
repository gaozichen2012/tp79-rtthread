#include "stm32f10x.h"
#include "device_config.h"
#include "gps.h"
#include "uart.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

gps_device_t  gps_device;
extern uint32_t SysTick_Cnt ;


extern device_uart_t  device_uart1;
/*
static uint8_t ptmk_check_sum(uint8_t *dat ,uint8_t datlen)
{
	uint8_t i,sum = 0;
	for(i=0; i<datlen; i++)
	{
		sum ^= dat[i];
	}
	return sum;
}
*/

int ddmm2dd(const char *ddmm, char *dd)  
{  
    if (NULL == ddmm || NULL == dd)  
    {  
        return -1;  
    }  
    int lenSrc = strlen(ddmm)+1;  
    int lenMm = 0;  
    int flag = 1;  
  
    memcpy(dd,ddmm,lenSrc);  
      
    char *pcMm;  
    double dMm;  
    int iMm;  
  
    /* ��pcMm��λ��С����λ�� */  
    pcMm = strstr(dd,".");  
  
    if (pcMm == NULL) /* ����С�������� */  
    {  
        pcMm = dd+strlen(dd)-2;  
        iMm = atoi(pcMm);  
        dMm = iMm /60.0;  
    }  
    else /* ����С�������� */  
    {     
        /* �ж� */  
        if (pcMm - dd > 2)  
        {  
            pcMm = pcMm - 2;  
        }  
        else /* û�ж�,ֻ�з� */  
        {  
            pcMm = dd;  
            flag = 0;  
        }  
        /* ���ַ���ת��Ϊ������ */  
        dMm = atof(pcMm);  
        /* ����ת��Ϊ�� */  
        dMm /= 60.0;  
    }  
    /* ��ת����ĸ�����ת��Ϊ�ַ��� */  
    sprintf(pcMm,"%lf",dMm);  
    if ( flag )  
    {  
        /* ȥ��С����ǰ���0 */  
        strcpy(pcMm,pcMm+1);  
    }  
    /* ����С�����6λ */  
    pcMm = strstr(dd,".");  
    lenMm = strlen(pcMm);  
    if ( lenMm > (6+2))  
    {  
        memset(pcMm+6+2,0,lenMm-6-2);  
    }  
      
    return 1;  
}  


void gps_init(void)
{
	uint32_t delay = 3000000;
	GPS_REST_ON();
	while(delay--);		
	GPS_REST_OFF();	
	
#ifdef DEVICE_GPS_UART1
	uart1_config();
#endif	
	 memset((uint8_t*)&gps_device,0,sizeof(gps_device_t));
	uart1_send_string((uint8_t*)"$PMTK\r\n",7);
	//GPS_mode_set_power_save(1);
	
}

//GPS˯��
void GPS_mode_set_power_save()
{
	uart1_send_string((uint8_t*)"$PMTK161,0*28\r\n",15);
	uart1_send_string((uint8_t*)"$PMTK161,0*28\r\n",15);
	uart1_send_string((uint8_t*)"$PMTK161,0*28\r\n",15);
}

void gps_uart_msg_deal(uint8_t *ptr)
{
	if(memcmp(ptr, "$GPGGA", 6) == 0) //GPS��λ����Ҫ����
	{
		uint8_t i,j,len = strlen((char*)ptr); //���֡255byte
		uint8_t frame_count = 0;//, ���и�	����֡������֡����
		gps_device.status = GPS_DEVICE_NORMAL;
		for(i=0,j=0;i<len;i++)
		{
			//��Ϣͷ��UTCʱ�䣬γ�ȣ�N/Sָʾ�����ȣ�E/Wָʾ����λ״̬��0 δ��λ��1ʵʱ��Ч��2�����Ч��������������Ŀ��ˮƽ���ȣ����߸̣߳���λ�����Ρ�������
			if(ptr[i] == ',')
			{
				if(frame_count == 2)//γ��  �� ddmm.mmmm  ת����dd.ddddd
				{
					ddmm2dd((const char*)ptr+j+1,(char*)gps_device.NS_value);	
				}
				else if(frame_count == 3)//γ������ N/S
				  gps_device.ns = (itude_t)*(ptr+i-1);
			  else if(frame_count == 4)//����  �� dddmm.mmmm  ת����ddd.ddddd
				{
					gps_device.EW_value[0] = *(ptr+j+1);
					ddmm2dd((const char*)ptr+j+2,(char*)&gps_device.EW_value[1]);						
				}
				else if(frame_count == 5)//γ������ W/E
				  gps_device.ew = (itude_t)*(ptr+i-1);					
				else if(frame_count == 6)//��λ���
				  gps_device.position_state = *(ptr+i-1);		
				else if(frame_count == 7)//������������
				{
					memset(gps_device.satellites_sum,0,4);
					memcpy(gps_device.satellites_sum,ptr+j+1,i-j-1);
				}		
				j = i;
				frame_count++;
			}
		}
	}
	else 	if(memcmp(ptr, "$GPGSV", 6) == 0) //��������״̬���
	{
		uint8_t i,j,len = strlen((char*)ptr); //���֡255byte
		uint8_t frame_count;//, ���и�	����֡������֡����		
		frame_count = 0;
		//uint8_t gsv_cnt;
		for(i=0,j=0;i<len;i++)
		{
			static uint8_t id ;
			//��Ϣͷ����Ϣ������Ϣ��ţ���������������id
			//���ǣ���λ�ǣ������ ......У���\r\n
			if(ptr[i] == ',')
			{
				if(frame_count == 1)//��Ϣ��Ŀ
				{
					//gsv_cnt =  (ptr[i-1]- '0');
				}										
				else if(frame_count == 4)//����id
				{
					id = (ptr[i-1]- '0') + (ptr[i-2]- '0')*10 ;
				}		
				else if(frame_count == 7)//�����
				{
					memset(gps_device.snr.SNR[id%32],0,4);
					if(i-j == 3)
						memcpy(gps_device.snr.SNR[id%32],&ptr[i-2],2);
					gps_device.snr.time[id%32] = SysTick_Cnt;
				}	
				else if(frame_count == 16)//����id
				{
					id = (ptr[i-1]- '0') + (ptr[i-2]- '0')*10 ;
				}		
				else if(frame_count == 19)//�����
				{
					memset(gps_device.snr.SNR[id%32],0,4);
					if(i-j == 3)
						memcpy(gps_device.snr.SNR[id%32],&ptr[i-2],2);
					gps_device.snr.time[id%32] = SysTick_Cnt;
				}	

				j = i;
				frame_count++;
			}
		}		
		
		
	}
}

void gps_task(void)
{
	uint8_t i;
	for(i=0; i<32; i++)//�����ź�ǿ�ȸ���ʱ�䣬���10���Ӷ�û�и��µ�ֵ����0
	{
		if(SysTick_Cnt - gps_device.snr.time[i] > 10000)
			gps_device.snr.time[i] = 0;
	}

	/***     gps  uart msg deal      ***/
	static uint8_t buf_temp_deal[257]  = {0};
	static uint8_t cmp_cnt = 0;		
	while(device_uart1.read_p != device_uart1.write_p )//������δ����
	{
		if(device_uart1.recv_buf[(device_uart1.read_p+cmp_cnt)%UART_RXBUF_LEN_MAX] == '\r' || 
			 device_uart1.recv_buf[(device_uart1.read_p+cmp_cnt)%UART_RXBUF_LEN_MAX] == '\n')//��⵽δ������������ \r\n��β������
		{	
			if(cmp_cnt >= 2)//ֻ�����ȴ���2��ָ��
			{
				memset(buf_temp_deal,0, 256);
				if(device_uart1.read_p+cmp_cnt < UART_RXBUF_LEN_MAX)//�������ѭ���洢
					memcpy(buf_temp_deal,&device_uart1.recv_buf[device_uart1.read_p], cmp_cnt);
				else
				{
					memcpy(buf_temp_deal,&device_uart1.recv_buf[device_uart1.read_p], (UART_RXBUF_LEN_MAX - device_uart1.read_p)%256);
					memcpy(&buf_temp_deal[UART_RXBUF_LEN_MAX - device_uart1.read_p],device_uart1.recv_buf, \
					(device_uart1.read_p+cmp_cnt)%UART_RXBUF_LEN_MAX%(256 - ((UART_RXBUF_LEN_MAX - device_uart1.read_p)%256)));
				}
				gps_uart_msg_deal(buf_temp_deal);	
			}
			device_uart1.read_p = (device_uart1.read_p+(cmp_cnt+1))%UART_RXBUF_LEN_MAX ;//�������
			cmp_cnt = 0;
		}
		else
		{
			if( ((device_uart1.read_p < device_uart1.write_p )&& (cmp_cnt+1 < device_uart1.write_p - device_uart1.read_p))||
					((device_uart1.read_p > device_uart1.write_p) && ( cmp_cnt+1 < (UART_RXBUF_LEN_MAX - device_uart1.read_p + device_uart1.write_p)) ))
				cmp_cnt++	;
			else
			{
				if(cmp_cnt >= 200)
				{
					device_uart1.write_p = 0;
					device_uart1.read_p = 0;
					cmp_cnt = 0;
				}						
				break;
			}
		}
	}	

}


uint8_t* gps_get_NS_value(void)
{
	return gps_device.NS_value;
}
uint8_t* gps_get_EW_value(void)
{
	return gps_device.EW_value;
}

itude_t gps_get_NS_type(void) 
{
	return gps_device.ns;
}

itude_t gps_get_EW_type(void) 
{
	return gps_device.ew;	
}

uint8_t gps_get_position_state(void) 
{
	return gps_device.position_state;
}
uint8_t* gps_get_satellites_sum(void) 
{
	return gps_device.satellites_sum;
}
uint8_t gps_get_status(void) 
{
	return gps_device.status;
}

gps_snr_t* gps_get_snr(void) 
{
	return &gps_device.snr;
}






