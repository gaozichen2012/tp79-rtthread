#ifndef __GPS_H_
#define __GPS_H_

#include "stm32f10x.h"
#include "nv3021.h"

#define GPS_REST_OFF()	LCD_REST_OFF()
#define GPS_REST_ON()   LCD_REST_ON()

typedef enum
{
	GPS_DEVICE_NOT_DETECT, //未检测到设备  
  GPS_DEVICE_NORMAL, //正常状态	
  GPS_DEVICE_MOVING, //移动状态
	
}gps_device_status_t;

typedef enum
{
	ITUDE_N = 'N',
	ITUDE_S = 'S',
	ITUDE_W = 'W',
	ITUDE_E = 'E',	
	
} itude_t;

typedef struct
{
	uint8_t SNR[32][4];  //信噪比，最多32个卫星
	uint32_t time[32];  //更新时间,超过时间 数值未更新，则清除信号强度为0
	//uint8_t number[32][4];  //卫星编号
	//仰角角度  90在头顶
	//方位角度  0~360 水平
	
}gps_snr_t,*gps_snr_pt;



typedef struct
{
	uint8_t NS_value[32]; //纬度 值
	itude_t ns;        //类型
  uint8_t EW_value[32];	//经度值
	itude_t ew;    //类型
	uint8_t position_state;  //定位状态 ‘1’ 定位成功，  ‘0’ 未定位   
	uint8_t satellites_sum[4];  //卫星数量	
  gps_snr_t  snr;   //卫星状态信息  信噪比
	gps_device_status_t status; 

}gps_device_t,*gps_device_pt;

void gps_init(void);
void gps_task(void);
void GPS_mode_set_power_save(void);


uint8_t* gps_get_NS_value(void) ; //纬度值
itude_t gps_get_NS_type(void) ;  //北纬OR南纬
uint8_t* gps_get_EW_value(void); //经度值
itude_t gps_get_EW_type(void) ;  //东经或西经
uint8_t gps_get_position_state(void) ; //定位状态，是否定位成功
uint8_t* gps_get_satellites_sum(void) ; //卫星总数
uint8_t gps_get_status(void) ;  //gps状态
gps_snr_t* gps_get_snr(void) ; //信号状态

#endif







