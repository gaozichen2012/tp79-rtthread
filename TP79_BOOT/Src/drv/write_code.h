#ifndef __WRITE_CODE_H_
#define __WRITE_CODE_H_


typedef enum
{
	WRITE_CODE_NOMAL,
	WRITE_CODE_READ_CFG,
	WRITE_CODE_WRITE_CFG,
	
}write_code_status_t;


write_code_status_t write_code_get_status(void);
void write_code_init(void);
void write_code_task(void);


#endif
