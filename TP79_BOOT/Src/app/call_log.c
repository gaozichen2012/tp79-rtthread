#include "stm32f10x.h"
#include "call_log.h"
#include "stm32f10x_flash.h"
#include "flash.h"
#include "string.h"
#include "my_string.h"

uint8_t call_log_read_cnt(void)
{
	uint32_t cnt_temp;
	uint8_t ret;
	Flash_Read(FLASH_ADDR_CALL_LOG_CNT,(uint8_t*)&cnt_temp, 4);
	if(cnt_temp > CALL_LOG_CONST_MAX)
		ret = CALL_LOG_CONST_MAX;
	else
		ret = cnt_temp;
	return ret;
}
void call_log_write_cnt(uint32_t cnt)
{
	Flash_Updata(FLASH_ADDR_CALL_LOG_CNT,(uint8_t*)&cnt, 4);	
}

void call_log_deinit(void)
{
	Flash_Erase(FLASH_ADDR_CALL_LOG_CNT,4*1024);
	call_log_write_cnt(0);
}

void call_log_init(void)
{
	if(call_log_read_cnt() > CALL_LOG_CONST_MAX)
		call_log_deinit();
}

void call_log_add(RTC_TimeTypeDef time, uint8_t *name, uint8_t *id,uint8_t type)
{
	uint32_t cnt;
	call_log_t call_log;	
	memcpy(call_log.caller_name,name,  POC_NAME_LENTH_MAX);
	memcpy(call_log.caller_id,id,  4);
	memcpy((uint8_t*)&call_log.time,(uint8_t*)&time,  sizeof(RTC_TimeTypeDef));
	call_log.call_type = type;
	Flash_Read(FLASH_ADDR_CALL_LOG_CNT,(uint8_t*)&cnt, 4);
	Flash_Updata(FLASH_ADDR_CALL_INFO_START + (cnt%CALL_LOG_CONST_MAX) * sizeof(call_log_t),(uint8_t*)&call_log, sizeof(call_log_t));	
	call_log_write_cnt(cnt+1);
}

void call_log_del(uint8_t index)
{
	uint32_t cnt;
	Flash_Read(FLASH_ADDR_CALL_LOG_CNT,(uint8_t*)&cnt, 4);	
	cnt = (cnt%CALL_LOG_CONST_MAX)-1;
	//将Index+1 *size后面的数据覆盖到Index位置
	Flash_Move(FLASH_ADDR_CALL_INFO_START + (cnt - index)*sizeof(call_log_t),FLASH_ADDR_CALL_INFO_START + ((cnt - index)+1)*sizeof(call_log_t) , FLASH_ADDR_CALL_INFO_END - (FLASH_ADDR_CALL_INFO_START + ((cnt - index)+1)*sizeof(call_log_t)));
	call_log_write_cnt(cnt);
}


void call_log_read(uint8_t index, call_log_t* log)
{
	uint8_t *p;
	uint32_t cnt;
	Flash_Read(FLASH_ADDR_CALL_LOG_CNT,(uint8_t*)&cnt, 4);
	p = (uint8_t*)(FLASH_ADDR_CALL_INFO_START + ((cnt - index) %CALL_LOG_CONST_MAX)*sizeof(call_log_t));
	memcpy(log, p,sizeof(call_log_t));
}






