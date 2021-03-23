#include "stm32f10x.h"
#include "device.h"

#include "flash.h"
#include "my_string.h"
#include "string.h"
#include "machine_config.h"

device_cfg_t  device_cfg ;
extern void Delay_Ms(uint32_t ms);


void device_config_save_flash(void)  //set 
{
	Flash_Erase(FLASH_ADDR_USER_START,2*1024);
	Delay_Ms(20);
	Flash_Write(FLASH_ADDR_USER_START, (uint8_t *)&device_cfg, sizeof(device_cfg_t));
	Delay_Ms(20);
}

device_cfg_pt device_config_read(void)
{
	Flash_Read(FLASH_ADDR_USER_START, (uint8_t *)&device_cfg, sizeof(device_cfg_t));
	return &device_cfg;
}

void device_init(void)
{
	Flash_Read(FLASH_ADDR_USER_START, (uint8_t *)&device_cfg, sizeof(device_cfg_t));
	if(device_cfg.init_flag != 0x66666666)
	{
		device_cfg.init_flag = 0x66666666;
		memcpy(device_cfg.ver,VER_SOFT,strlen(VER_SOFT));
		device_config_save_flash();
	}
}


