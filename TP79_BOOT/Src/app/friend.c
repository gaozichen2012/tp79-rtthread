#include "stm32f10x.h"
#include "friend.h"
#include "stm32f10x_flash.h"
#include "flash.h"
#include "string.h"



//读取好友个数 ，最大256
uint32_t friend_read_cnt(void)
{
	uint32_t cnt;
	Flash_Read(FLASH_ADDR_FRIEND_CNT,(uint8_t*)&cnt,4);
	return cnt%256;
}

static void friend_write_cnt(uint32_t cnt)
{
	Flash_Updata(FLASH_ADDR_FRIEND_CNT,(uint8_t*)&cnt,4);
}


void friend_deinit(void)
{
	Flash_Erase(FLASH_ADDR_FRIEND_INFO_START,8*1024);	
	friend_write_cnt(0);	
}

void friend_init(void)
{
	if(friend_read_cnt() > 500)
		friend_write_cnt(0);
}

uint8_t friend_add(friend_t fd)
{
	uint32_t cnt,i;
	friend_t fd_temp;
	cnt = friend_read_cnt()%256;
	if(cnt+1 < 256)
	{
		for(i=0;i<cnt;i++)
		{
			friend_read( i,&fd_temp);
			if(memcmp(fd_temp.friend_id, fd.friend_id,4) == 0)
				return 2;
		}
		Flash_Updata(FLASH_ADDR_FRIEND_INFO_START + cnt*(sizeof(friend_t)),(uint8_t*)&fd,sizeof(friend_t));
		friend_write_cnt(cnt+1);
		return 1;
	}
	else
		return 0;
}

uint8_t friend_del(uint8_t index)
{
	uint32_t cnt;
	uint8_t  temp[32];
	cnt = friend_read_cnt()%256;
	if(index <= cnt  && cnt >=1 )
	{
		Flash_Read(FLASH_ADDR_FRIEND_INFO_START+(cnt-1)*32,temp,32);
		Flash_Updata(FLASH_ADDR_FRIEND_INFO_START + index*(sizeof(friend_t)),temp,32);		
		friend_write_cnt(cnt-1);		
		return 1;
	}
	return 0;	
}


//index 从0开始
uint8_t friend_read(uint8_t index, friend_t* fd)
{
	Flash_Read(FLASH_ADDR_FRIEND_INFO_START + index*(sizeof(friend_t)),(uint8_t*)fd,sizeof(friend_t));
	return 1;
}






