#ifndef __CALL_LOG_H_
#define __CALL_LOG_H_

#include "poc.h"
#define CALL_LOG_CONST_MAX   (63)

//64byte 
typedef struct
{
	uint8_t caller_name[POC_NAME_LENTH_MAX]; //網請氪靡備 26
	uint8_t caller_id[4]; //網請氪id  4
	RTC_TimeTypeDef time;  //網請奀潔  4
	uint8_t call_type;    //網請濬倰ㄛ  翋請 1ㄛ 掩請 0
	uint8_t reserve[29];
}call_log_t,*call_log_pt;

void call_log_init(void);
void call_log_deinit(void);
void call_log_add(RTC_TimeTypeDef time, uint8_t *name, uint8_t *id,uint8_t type);
void call_log_del(uint8_t index);
void call_log_read(uint8_t index, call_log_t* log);
uint8_t call_log_read_cnt(void);

#endif





