#ifndef __POC_H_
#define __POC_H_

#include "device_config.h"

#define POC_EC20

#define POC_TTS_STATUS_NOMAL	0
#define POC_TTS_STATUS_BUSY		1


#define POC_NET_MODE_4G	1
#define POC_NET_MODE_3G	2
#define POC_NET_MODE_2G	3

#define POC_NAME_LENTH_MAX  (26)



#define AUDIO_GPIO_POC_RST_PORT   GPIOD
#define AUDIO_GPIO_POC_RST_PIN    GPIO_Pin_9		///---ON/OFF 引脚

#define DEVICE_GPIO_POWER_PORT   GPIOA
#define DEVICE_GPIO_POWER_PIN   GPIO_Pin_0

#define AUDIO_GPIO_AP_EN_PORT   GPIOC
#define AUDIO_GPIO_AP_EN_PIN   GPIO_Pin_14

#define POC_SW_OFF()	 GPIO_ResetBits(AUDIO_GPIO_POC_RST_PORT, AUDIO_GPIO_POC_RST_PIN)
#define POC_SW_ON()  	 GPIO_SetBits(AUDIO_GPIO_POC_RST_PORT, AUDIO_GPIO_POC_RST_PIN)

#define POC_POWER_ON()	 GPIO_SetBits(DEVICE_GPIO_POWER_PORT, DEVICE_GPIO_POWER_PIN)
#define POC_POWER_OFF()  GPIO_ResetBits(DEVICE_GPIO_POWER_PORT, DEVICE_GPIO_POWER_PIN)

#define POC_CMD_AT  	"AT\r\n"
#define POC_CMD_ATE0  "ATE0\r\n"
#define POC_CMD_CPIN  "AT+CPIN?\r\n"
#define POC_CMD_POC_OPENING   "AT+POC=000000010101\r\n"
#define POC_CMD_KEY_SOUND		"AT+ZTONE=1,1,880,3,1\r\n"
#define POC_CMD_CCLK		"AT+CCLK?\r\n"


#define POC_MEMBER_SIMPLE_MSG "AT+POC=10000020\r\n"

#ifdef POC_ME3610
#define POC_CMD_CSQ   "AT+CSQ\r\n"
#define AT_CMD_POWER_OFF  "AT+ZTURNOFF\r\n"
#endif

#ifdef POC_MC8332
#define AT_CMD_POWER_OFF  "AT+ZPWROFF\r\n"
#define POC_CMD_CSQ   "AT+CSQ?\r\n"
#endif

#ifdef POC_EC20
#define POC_CMD_CREG "AT+QNWINFO\r\n"
#define AT_CMD_POWER_OFF  "AT+QPOWD=0\r\n"
#define AT_CMD_POWER_RST  "AT+CFUN=1,1\r\n"
#define POC_CMD_CSQ   "AT+CSQ\r\n"
#define AT_CMD_QIOTRPT "AT+QIOTRPT=1,0\r\n"
#define AT_CMD_POC_REFRESH_GPS "AT+QGPSGNMEA=\"GGA\"\r\n"

#endif


#define POC_CMD_REFRESH_GROUP  "AT+POC=0D0000\r\n"
#define POC_CMD_POC_LAND		"AT+POC=030000\r\n"
#define POC_CMD_POC_LAND_OUT		"AT+POC=050000\r\n"


//#define POC_CMD_CREG "AT+ZPAS?\r\n"
#define POC_CMD_ZADSET "AT+ZADSET=d\r\n"

#define AT_CMD_GET_MEID "AT^MEID\r\n"

#define AT_CMD_POC_PARA_GET  "AT+POC=020000\r\n"
					
typedef enum
{
	ATCMD_TEST,
	ATCMD_ATE0,
	ATCMD_CPIN,
	ATCMD_POC_OPENING,
	ATCMD_POC_KEY_SOUND,
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
	ATCMD_POC_PARA_GET,
}poc_cmd_t;

typedef enum
{
	POC_DEVICE_STATUS_STARTING,   //开机状态
	POC_DEVICE_STATUS_CHECKED_OK,  //CHECKED OK 检测到模块
	POC_DEVICE_STATUS_ENTER_OK,   //发送进入POC
	POC_DEVICE_STATUS_SIM_OK,  //检测到sim  OK
	POC_DEVICE_STATUS_SIM_ERROR,  //检测到sim  ERROR
	POC_DEVICE_STATUS_LANDING,  //登录中	
	POC_DEVICE_STATUS_LAND_OUT,  //退出登录，离线	
	POC_DEVICE_STATUS_LAND_SUCCESSED,  //登录成功
	POC_DEVICE_STATUS_SHUTDOWN, //关机
	POC_DEVICE_STATUS_SHUTDOWNING,
	POC_DEVICE_VER_UPGRADE,   //升级	
	POC_DEVICE_VER_SAME,   //版本一致
	POC_DEVICE_VER_DWONLOAD,
	POC_DEVICE_VER_DWONLOAD_SUCCESS,	
}poc_device_status_t;

typedef enum
{
	POC_CALL_NOMAL,  
	POC_CALLING,   //正在呼叫
	POC_CALL_SUCCESS, //呼叫成功
	POC_CALL_FAILED, //呼叫失败
	POC_CALL_EXIT,  //退出临时呼叫
	POC_BE_CALLED,   //正在被呼叫
}poc_call_status_t;

typedef struct
{
	poc_device_status_t status;  //poc 状态  poc_device_status_t
	uint8_t down_status;
	uint16_t down_pack_cnt;//当前下载包的序号
	uint16_t pack_sum;//包的个数
	uint8_t  server_soft_ver[32];
	uint8_t  server_soft_id[4];	
	
}poc_device_t,*poc_device_pt;


void poc_init(void);
void poc_send_cmd(poc_cmd_t poc_cmd);  //发送AT指令  poc_cmd_t
void poc_task(void);

poc_device_pt poc_get_para(void);//POC参数

#endif
