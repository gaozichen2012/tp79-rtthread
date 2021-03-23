#ifndef __DEVICE_H_
#define __DEVICE_H_


typedef struct
{
	uint32_t init_flag;
	uint8_t  ver[32];   //°æ±¾
}device_cfg_t,*device_cfg_pt;

void device_init(void);
void device_config_set_default(void);
void device_config_read_flash(void);
void device_config_save_flash(void);  //set 
device_cfg_pt device_config_read(void);

#endif



