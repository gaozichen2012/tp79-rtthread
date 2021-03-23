#include "stm32f10x.h"
#include "ui.h"

#include "adc.h"
#include "string.h"
#include "GT21L16S2Y.H"
#include "poc.h"
#include "my_string.h"
#include "stm32f10x_rtc.h"
#include "adc.h"
#include "application.h"
#include "friend.h"
#include "gps.h"
#include "device.h"
#include "call_log.h"
#include "write_code.h"

#define CHINESE_TYPE_11x12 1
#define CHINESE_TYPE_15x16 2

extern const unsigned char logoImg[40200];
extern const unsigned char gImage_chengyuan[1218];
extern const unsigned char gImage_trumpet[338];
extern const unsigned char gImage_qunzu[1248] ;
extern const unsigned char gImage_haoyou[1200];
extern const unsigned char gImage_jilu[1344];
extern const unsigned char gImage_gps[944];
extern const unsigned char gImage_setup[1456];
extern const unsigned char gImage_sim[92] ;
extern const unsigned char gImage_user[];
extern const unsigned char GPS_dat[24];
extern const unsigned char right_dat[24] ;
extern const unsigned char gImage_chengyuan2[800] ;
extern const unsigned char gImage_chengyuan17X24[816];//17*24
extern const unsigned char gImage_qunzu22x24[1056];
extern const unsigned char  gImage_jiantou[];
extern uint32_t SysTick_Cnt ;

typedef struct
{
	uint8_t disp_text[64];
	uint32_t speed;
	uint32_t disp_timer;
	uint8_t textlen;
	uint8_t texttype;	
	uint8_t roll_cnt;
	uint8_t x;
	uint8_t y;
	uint16_t text_color;
	uint16_t black_color;
}ui_text_roll_t,*ui_text_roll_pt;

static ui_text_roll_t	text_roll[3];


/******     gps  刷新          ********/
static void ui_disp_ican_gps()
{
	static uint8_t old_signal = 55,cur_signal;
	cur_signal = gps_get_position_state();
	
	if(old_signal != cur_signal)
	{
		old_signal = cur_signal;
		if(cur_signal == '1' || cur_signal == '2')
		{
			lcd_disp_bit1bmp_SingleByte(120,3,GPS_dat,12,12,COLOR_WIGHT,COLOR_BLACK);	
		}
		else
		{
			lcd_disp_bit1bmp_SingleByte(120,3,GPS_dat,12,12,COLOR_RED,COLOR_BLACK);				
		}
	}	
}


/******      电池电量 刷新          ********/
static void ui_disp_ican_bat()
{
  uint16_t BatLevelValue[13]={2850,3020,3080,3210,3260,3350,3470,3570,3670,3790,3880,4100,5000};//,2800关机，最后的5000是人为添加的值
	uint16_t Color = COLOR_WIGHT,i;
	static uint16_t cur_level, old_level = 99;
	cur_level = get_adc_value(0);
	for(i=0; i<13; i++)
	{
		if(cur_level < BatLevelValue[i])
			break;
	}
	cur_level = i;
	if(old_level != cur_level)
	{
		if(cur_level > old_level && (cur_level == old_level+1))
		{
			if( get_adc_value(0) - BatLevelValue[old_level]  < 50)
				return;
		}
		old_level = cur_level;
		if(cur_level<2)
			Color = COLOR_RED;
		lcd_disp_range(140,3,17,10,2,Color);
		lcd_disp_box(157,6,Color,2,4);
		lcd_disp_box(143,6,COLOR_BLACK,2,4);
		lcd_disp_box(146,6,COLOR_BLACK,2,4);
		lcd_disp_box(149,6,COLOR_BLACK,2,4);
		lcd_disp_box(152,6,COLOR_BLACK,2,4);
		if(cur_level>=2)
			lcd_disp_box(143,6,Color,2,4);
		if(cur_level>=4)
			lcd_disp_box(146,6,Color,2,4);
		if(cur_level>=6)
			lcd_disp_box(149,6,Color,2,4);
		if(cur_level>=8)
			lcd_disp_box(152,6,Color,2,4);
	}

}	
/******      信号强度 刷新          ********/
static void ui_disp_ican_signal()
{
	poc_device_pt poc_para = poc_get_para();
	static uint8_t old_signal = 99,cur_signal;
	cur_signal = poc_para->csq;
	if(old_signal != cur_signal)
	{
		old_signal = cur_signal;
		lcd_disp_box(9,10,COLOR_BLACK,2,3);
		lcd_disp_box(13,8,COLOR_BLACK,2,5);
		lcd_disp_box(17,6,COLOR_BLACK,2,7);
		lcd_disp_box(21,4,COLOR_BLACK,2,9);
		if(cur_signal > 3)
			lcd_disp_box(9,10,COLOR_WIGHT,2,3);
		if(cur_signal > 10)
			lcd_disp_box(13,8,COLOR_WIGHT,2,5);
		if(cur_signal > 18)
			lcd_disp_box(17,6,COLOR_WIGHT,2,7);
		if(cur_signal > 24)
			lcd_disp_box(21,4,COLOR_WIGHT,2,9);	
	}

}


static void ui_disp_clr(uint16_t color)
{
	 lcd_color_clr(0,0,color, 160, 132);	
}

//fun: 显示15*16 中文
//输入x,y,坐标，中文2302， 背景色，文字颜色
//单行显示，超出则丢弃
void ui_disp_china_15x16(uint16_t x,uint16_t y, char *gb2312_dat, uint16_t black_color, uint16_t text_color)
{
  uint16_t i,j,lenth;
	uint8_t disp_buf[32] = {0};
	lenth = strlen(gb2312_dat);
	for(i=0; i<lenth; i++)
	{
		if(gb2312_dat[i] >= 0x20 && gb2312_dat[i] <= 0x7e)
		{
			gt21l16s2_get_ascii_8x16(disp_buf,*(gb2312_dat+i));
			//把最下面的三行空白点移到上面去
			for(j=0;j<8;j++)
			{
				disp_buf[j+8] <<= 3 ;
				disp_buf[j+8] |= ((disp_buf[j]>>5) & 0x07);					
				disp_buf[j] = ((disp_buf[j]<<3)&0xF8);
			}
			lcd_disp_bit1bmp_SingleByte(x+i*8,y,disp_buf,8,8,text_color,black_color);			
			lcd_disp_bit1bmp_SingleByte(x+i*8,y+8,&disp_buf[8],8,8,text_color,black_color);			
		}			
		else
		{
			if(gt21l16s2_get_gb2312_15x16(disp_buf,( uint8_t*)(gb2312_dat+i)))	
			{
				lcd_disp_bit1bmp_SingleByte(x+i*8,y,disp_buf,16,8,text_color,black_color);			
				lcd_disp_bit1bmp_SingleByte(x+i*8,y+8,&disp_buf[16],16,8,text_color,black_color);	
			  i++;
			}
			else//出错了，找不到字库
			{
			}
		}			
	}
}


//fun: 显示11*12 中文
//输入x,y,坐标，中文2302， 背景色，文字颜色
//单行显示，超出则丢弃
static void ui_disp_china_11x12(uint16_t x,uint16_t y, char *gb2312_dat, uint16_t black_color, uint16_t text_color)
{
  uint16_t i,lenth;
	uint8_t disp_buf[32] = {0};
	lenth = strlen(gb2312_dat);
	for(i=0; i<lenth; i++)
	{
		if(gb2312_dat[i] >= 0x20 && gb2312_dat[i] <= 0x7e)
		{
			gt21l16s2_get_ascii_6x12(disp_buf,*(gb2312_dat+i));
			lcd_disp_bit1bmp_SingleByte(x+i*6,y,disp_buf,6,8,text_color,black_color);			
			lcd_disp_bit1bmp_SingleByte(x+i*6,y+8,&disp_buf[6],6,5,text_color,black_color);				
		}			
		else
		{
			if(gt21l16s2_get_gb2312_11x12(disp_buf,( uint8_t*)(gb2312_dat+i)))	
			{
				lcd_disp_bit1bmp_SingleByte(x+i*6,y,disp_buf,12,8,text_color,black_color);			
				lcd_disp_bit1bmp_SingleByte(x+i*6,y+8,&disp_buf[12],12,4,text_color,black_color);		
				i++;
			}
			else//出错了，找不到字库
			{
			}
		}			
	}
}

//输入x,y,坐标，中文2302， 背景色，文字颜色
//单行显示，滚动显示
//设置滚动的参数，在Ui task里面实现滚动,最多设置3行滚动显示
static void ui_disp_chinese_roll(uint8_t text_type, uint8_t x,uint8_t y, char *gb2312_dat, uint16_t black_color, uint16_t text_color,uint8_t textlen, uint32_t roll_speed)
{
	uint8_t i;
	for(i=0; i<3; i++)
	{
		if(text_roll[i].speed == 0)
		{
			text_roll[i].speed = roll_speed;
			text_roll[i].textlen = textlen;
			text_roll[i].text_color = text_color;
			text_roll[i].black_color = black_color;
			text_roll[i].x = x;
			text_roll[i].y = y;
			text_roll[i].texttype = text_type;
			memcpy(text_roll[i].disp_text, gb2312_dat, strlen(gb2312_dat)%64);
			break;
		}
	}
}

static void ui_disp_chinese_roll_stop(void)
{
	memset(text_roll,0,sizeof(text_roll));
}


/******       ui_disp_ican_date 刷新          ********/
static void ui_disp_ican_date(uint8_t new_flag)
{
	RTC_DateTypeDef data;
	const char week[][3] = {"一","二","三","四","五","六","日"};
	static uint8_t cur_m[3] = {0} ,bef_m[3] = {0};
	static uint8_t cur_d[3] = {0} ,bef_d[3] = {0};
	static uint8_t cur_w ,bef_w;
	RTC_GetDate(RTC_Format_BCD, &data);	
	cur_m[0] = ((data.RTC_Month >> 4)&0x0f) + '0';
	cur_m[1] = (data.RTC_Month & 0x0f) + '0';
	cur_d[0] = ((data.RTC_Date >> 4)&0x0f)+ '0';
	cur_d[1] = (data.RTC_Date & 0x0f)+ '0';
	cur_w = data.RTC_WeekDay;
	
	if(cur_m[0] != bef_m[0] || cur_m[1] != bef_m[1] || new_flag)		
	{
		memcpy(bef_m, cur_m,2);
		ui_disp_china_11x12(112,113,(char*)cur_m,COLOR_BLUE,COLOR_WIGHT );			
	}
	if(cur_d[0] != bef_d[0] || cur_d[1] != bef_d[1] || new_flag)		
	{
		memcpy(bef_d, cur_d,2);
		ui_disp_china_11x12(136,113,(char*)cur_d,COLOR_BLUE,COLOR_WIGHT);					
	}
	if(cur_w != bef_w || new_flag)		
	{
		bef_w = bef_w;
		ui_disp_china_11x12(85,113,(char*)week[cur_w],COLOR_BLUE,COLOR_WIGHT );					
	}				
}

static void ui_disp_poc_group_refush(uint8_t refush_flag)//群组更新
{
	poc_device_pt poc_para = poc_get_para();
	static uint8_t old_group[26] = "                  ";
	static uint8_t old_land_flag = 1;
	if(poc_para->status == POC_DEVICE_STATUS_LAND_SUCCESSED )//POC在线
	{
		if(memcmp(poc_para->group_name_cur,old_group,POC_NAME_LENTH_MAX) != 0 || refush_flag || old_land_flag == 0)//群组变化，或者登录状态变化
		{
			memcpy(old_group,poc_para->group_name_cur,POC_NAME_LENTH_MAX);
			ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);		
			lcd_disp_box(0,33,COLOR_BLUE, 160,2); 	
			if(poc_para->group_name_cur[1] != 0)
				ui_disp_china_15x16(80-(strlen((char*)poc_para->group_name_cur) * 4),18,(char*)poc_para->group_name_cur,COLOR_BLUE,COLOR_WIGHT );	//显示登录群组	
			else
				ui_disp_china_15x16(52,18,"*未登录*",COLOR_BLUE,COLOR_RED );	//显示登录群组	
			ui_disp_china_15x16(80-(strlen((char*)poc_para->user_name) * 4),82,(char*)poc_para->user_name,COLOR_WIGHT, COLOR_BLUE);
		}	
		old_land_flag = 1;		
	}
	else //POC离线
	{
		if(old_land_flag == 1 || refush_flag)
		{
			old_land_flag = 0;
			ui_disp_china_15x16(80-(strlen((char*)poc_para->user_name) * 4),82,(char*)poc_para->user_name,COLOR_WIGHT, COLOR_GRAY);		//显示灰色用户名		
			ui_disp_china_15x16(10,18,(char*)"     *离线*       ",COLOR_BLUE,COLOR_WIGHT );	//显示登录群组	
		}			
	}	
}
/******      显示主菜单            ********/
static void ui_menu_main(void)
{
	lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
	ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);		
	lcd_disp_box(0,33,COLOR_BLUE, 160,2); 					
	lcd_color_clr(0,35,COLOR_WIGHT,160,78);	
	lcd_disp_bmp_SingleByte(67,52,gImage_chengyuan,21, 29);		//显示当前用户Id头像
	lcd_disp_box(0,111,COLOR_BLUE, 160,128); 	
	ui_disp_china_11x12(5,113,"菜单", COLOR_BLUE,COLOR_WIGHT);
	ui_disp_china_11x12(59,113,"星期", COLOR_BLUE,COLOR_WIGHT);	
	ui_disp_china_11x12(112,113,"  月  日", COLOR_BLUE,COLOR_WIGHT);	
	ui_disp_poc_group_refush(1);//刷新群组
	ui_disp_ican_date(1);	//刷新日期
}

/******       开机检测到POC，欢迎菜单          ********/
static void ui_menu_enter_poc(void)
{
	ui_disp_clr(COLOR_WIGHT);
	lcd_disp_box(0,0,COLOR_BLACK, 160,16); //显示黑块 16*160
	lcd_disp_tan(2,2,12,2,7,7,1,COLOR_WIGHT);//显示三角形
	lcd_disp_box(6,7,COLOR_WIGHT, 2,6); //显示三角形下面的竖线			
	ui_disp_ican_signal();
	ui_disp_ican_bat();
	ui_disp_china_15x16(16,50,"欢迎使用对讲系统", COLOR_WIGHT, COLOR_BLUE);	
}

/******      gps 刷新          ********/
static void  ui_disp_gps_info( uint8_t en)
{
  static gps_device_t  gps_old_info = {0};
	static uint8_t signal_flag = 0;
	uint8_t i,j;
	if(gps_get_position_state() == '1' || gps_get_position_state() == '2' )
	{
		if(signal_flag == 0)
		{
			signal_flag = 1;
			ui_disp_china_11x12(80,114,"GPS信号丢失", COLOR_BLUE,COLOR_BLUE);
		}
		//刷新经纬度
		if(memcmp(gps_old_info.NS_value, gps_get_NS_value(), 32) ||  en)
		{
			memcpy(gps_old_info.NS_value, gps_get_NS_value(),32);
			ui_disp_china_11x12(60,40,(char*)(gps_get_NS_value()), COLOR_WIGHT,COLOR_BLUE);	
		}
		if(gps_old_info.ns != gps_get_NS_type() ||  en)
		{
			char msg[2] = {0};
			msg[0] = gps_get_NS_type();
			gps_old_info.ns = gps_get_NS_type();
			ui_disp_china_11x12(140,40,msg, COLOR_WIGHT,COLOR_BLUE);	
		}		
		//刷新经纬度
		if(memcmp(gps_old_info.EW_value, gps_get_EW_value(), 32) ||  en)
		{
			memcpy(gps_old_info.EW_value, gps_get_EW_value(),32);
			ui_disp_china_11x12(60,56,(char*)(gps_get_EW_value()), COLOR_WIGHT,COLOR_BLUE);	
		}		
		if(gps_old_info.ew != gps_get_EW_type() ||  en)
		{
			char msg[2] = {0};
			msg[0] = gps_get_EW_type();
			gps_old_info.ew = gps_get_EW_type();
			ui_disp_china_11x12(140,56,msg, COLOR_WIGHT,COLOR_BLUE);	
		}				
	
		
		//刷新卫星数量
		if(memcmp(gps_old_info.satellites_sum, gps_get_satellites_sum(), 4) ||  en)
		{
			memcpy(gps_old_info.satellites_sum, gps_get_satellites_sum(),4);
			lcd_disp_box(60,72,COLOR_WIGHT,32,13);
			ui_disp_china_11x12(60,72,(char*)(gps_get_satellites_sum()), COLOR_WIGHT,COLOR_BLUE);	
		}			
		//刷新信号强度
		for(i=0,j=0; i<32; i++)
		{
			static gps_snr_t snr_t = {0};
			gps_snr_pt gps_snr = gps_get_snr();
			if(gps_snr->time[i] > 0 )//显示最新的信号强度值
			{		
				if(memcmp(snr_t.SNR[i],gps_snr->SNR[i],4) != 0 || en)
				{					
					memcpy(snr_t.SNR[i],gps_snr->SNR[i],4);
					if(	j<=4)
						ui_disp_china_11x12(60+(j++)*20,88,(char*)gps_snr->SNR[i], COLOR_WIGHT,COLOR_BLUE);	
					else if(j<=10)
						ui_disp_china_11x12(60+(j++ - 5)*20,100,(char*)gps_snr->SNR[i], COLOR_WIGHT,COLOR_BLUE);	
				}
			}
		}
	}
	else
	{
		if(signal_flag == 1 || en)
		{
			signal_flag = 0;
			ui_disp_china_11x12(80,114,"GPS信号丢失", COLOR_BLUE,COLOR_RED);
		}
	}

 
}


/******       ican time 刷新          ********/
static void ui_disp_ican_time(void)
{
		RTC_TimeTypeDef cur_time;
		static uint8_t cur_h[3] = {0} ,bef_h[3] = {0};
		static uint8_t cur_m[3] = {0} ,bef_m[3] = {0};
		static uint8_t cur_s[3] = {0} ,bef_s[3] = {0};
		RTC_GetTime(RTC_Format_BCD, &cur_time);	
		cur_s[0] = ((cur_time.RTC_Seconds >> 4)&0x0f) + '0';
		cur_s[1] = (cur_time.RTC_Seconds & 0x0f) + '0';
		cur_m[0] = ((cur_time.RTC_Minutes >> 4)&0x0f)+ '0';
		cur_m[1] = (cur_time.RTC_Minutes & 0x0f)+ '0';
		if(cur_time.RTC_H12 == RTC_H12_AM)
		{
			cur_h[0] = ((cur_time.RTC_Hours >> 4)&0x0f)+ '0';
			cur_h[1] = (cur_time.RTC_Hours & 0x0f) + '0';		
		}
		else
		{
			cur_h[0] = (((cur_time.RTC_Hours+12) >> 4)&0x0f) + '0';
			cur_h[1] = ((cur_time.RTC_Hours+12 )&0x0f)+ '0';				
		}
    if(cur_h[0] != bef_h[0] || cur_h[1] != bef_h[1])		
		{
			memcpy(bef_h, cur_h,2);
			ui_disp_china_11x12(50,3,(char*)cur_h,COLOR_BLACK,COLOR_WIGHT );	
			ui_disp_china_11x12(62,3,":",COLOR_BLACK,COLOR_WIGHT );				
		}
    if(cur_m[0] != bef_m[0] || cur_m[1] != bef_m[1])		
		{
			memcpy(bef_m, cur_m,2);
			ui_disp_china_11x12(68,3,(char*)cur_m,COLOR_BLACK,COLOR_WIGHT);	
			ui_disp_china_11x12(80,3,":",COLOR_BLACK,COLOR_WIGHT );				
		}
    if(cur_s[0] != bef_s[0] || cur_s[1] != bef_s[1])		
		{
			memcpy(bef_s, cur_s,2);
			ui_disp_china_11x12(86,3,(char*)cur_s,COLOR_BLACK,COLOR_WIGHT );					
		}			
}

/******     按键对讲小喇叭    ********/

static void  ui_disp_poc_talk_pttkey()
{
	static uint8_t talk_status = 0;	
	static uint32_t timer;
	poc_device_pt poc_para = poc_get_para();
	if(talk_status == 0)
	{
		if( poc_para->key_ptt_talk_flag)//进入对讲
		{
			lcd_disp_bmp_SingleByte(110,40,gImage_trumpet,14, 12);	
			ui_disp_china_11x12(128,41,"0s",COLOR_WIGHT, COLOR_RED);			
			talk_status = 1;	
			timer = SysTick_Cnt;
		}
	}
	else
	{
		if(0 == poc_para->key_ptt_talk_flag)//退出对讲
		{
			lcd_disp_box(110,40,COLOR_WIGHT,14,12);
			ui_disp_china_11x12(128,40,"      ",COLOR_WIGHT, COLOR_RED);
			talk_status = 0;
		}	
		else//在对讲中要读秒
		{
			static uint8_t cur_sec = 0,old_sec = 0;
			cur_sec = ((SysTick_Cnt - timer)/1000)%255;
			if(old_sec != cur_sec)
			{
				char disp_sec[4] = "00s";
				old_sec = cur_sec;
				disp_sec[0] = (cur_sec /10) + '0';
				disp_sec[1] = cur_sec%10  + '0';
				if(disp_sec[0]  == '0')
				{
					disp_sec[0] = disp_sec[1];
					disp_sec[1] = 's';
					disp_sec[2] = 0;
				}			
				ui_disp_china_11x12(128,41,disp_sec,COLOR_WIGHT, COLOR_RED);
			}
		}
	}
}


/******      群组成员对呼叫单呼对讲小喇叭       ********/
static void ui_disp_poc_call_group(void)
{
	static uint8_t talk_status = 0;	
	static uint32_t timer;
	poc_device_pt poc_para = poc_get_para();	
	if(talk_status == 0)
	{
		if(poc_para->talker_falg)
		{
			lcd_disp_bmp_SingleByte(30,40,gImage_trumpet,14, 12);	
			ui_disp_china_11x12(48,41,"0s",COLOR_WIGHT, COLOR_RED);			
			talk_status = 1;	
			timer = SysTick_Cnt;
		}
	}
	else
	{
		if(!poc_para->talker_falg)
		{
			lcd_disp_box(30,40,COLOR_WIGHT,14,12);
			ui_disp_china_11x12(48,40,"      ",COLOR_WIGHT, COLOR_RED);
			talk_status = 0;
		}	
		else//在对讲中要读秒
		{
			static uint8_t cur_sec = 0,old_sec = 0;
			cur_sec = ((SysTick_Cnt - timer)/1000)%255;
			if(old_sec != cur_sec)
			{
				char disp_sec[4] = "00s";
				old_sec = cur_sec;
				disp_sec[0] = (cur_sec /10) + '0';
				disp_sec[1] = cur_sec%10  + '0';
				if(disp_sec[0]  == '0')
				{
					disp_sec[0] = disp_sec[1];
					disp_sec[1] = 's';
					disp_sec[2] = 0;
				}			
				ui_disp_china_11x12(48,41,disp_sec,COLOR_WIGHT, COLOR_RED);
			}
		}
	}	
}

/******      群组成员对讲小喇叭       ********/
static void ui_disp_poc_talk_group(void)
{
	static uint8_t talk_status = 0;
	poc_device_pt poc_para = poc_get_para();	
	if(talk_status == 0)
	{
		if(poc_para->talker_falg)
		{
			lcd_disp_bmp_SingleByte(1,40,gImage_trumpet,14, 12);	
			ui_disp_china_11x12(18,40,(char*)poc_para->talker_id,COLOR_WIGHT, COLOR_RED);		
			talk_status = 1;			
		}
	}
	else
	{
		if(!poc_para->talker_falg)
		{
			lcd_disp_box(1,40,COLOR_WIGHT,14,12);
			ui_disp_china_11x12(18,40,"                  ",COLOR_WIGHT, COLOR_BLUE);	
		  talk_status = 0;
		}		
	}
}


/******      main  选择框刷新      ********/
static void ui_disp_menu_main_select(uint8_t en)
{
	static uint16_t old_select = 0, cur_select;
	cur_select = application_get_menu_select();
	if(old_select != cur_select || en)
	{
		const char menu_list[][18] = {"群组","成员","好友","记录","GPS","设置"};
		lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
		ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);		
		lcd_disp_box(0,33,COLOR_BLUE, 160,2);		
		ui_disp_china_15x16(64-strlen(menu_list[cur_select])/2,18,(char*)menu_list[cur_select],COLOR_BLUE,COLOR_WIGHT );	
		lcd_disp_range(8 + (old_select%3)*56,41 + (old_select/3)*35,32,32,1,COLOR_WIGHT);
		lcd_disp_range(8 + (cur_select%3)*56,41 + (cur_select/3)*35,32,32,1,COLOR_RED);
		old_select = cur_select;		
	}
}
/******    输入10进制数据在屏上显示阿拉伯数字     ********/
static void ui_disp_itoa(uint16_t num,uint8_t startx,uint8_t starty,uint16_t bk_color,uint16_t num_color)
{
	char cnt[4] = {0};
	if(num > 99)
	{
		cnt[0] = (num/100) + '0';
		cnt[1] = (num/10) + '0';		
		cnt[2] = (num%10)  + '0';	
		cnt[3] = 0;		
	}
	else if(num > 9)
	{
		cnt[0] = (num/10) + '0';		
		cnt[1] = (num%10)  + '0';	
		cnt[2] = 0;		
	}	
	else
	{
		cnt[0] = (num%10)  + '0';	
		cnt[1] = 0;				
	}
	if(num > 9)
		ui_disp_china_11x12(startx - 3,starty,cnt, bk_color,num_color);	
	else
		ui_disp_china_11x12(startx,starty,cnt, bk_color,num_color);			
}


void 	ui_disp_chinese_roll_task(void)
{
	uint8_t i;
	uint32_t speed;
	uint8_t disp_buf[64] ;
	uint8_t clear_buf[64] = "                                       ";
	for(i=0; i<3; i++)
	{
		if(text_roll[i].speed > 0)
		{
			clear_buf[text_roll[i].textlen] = 0;
			if(text_roll[i].roll_cnt == 1)
				speed = text_roll[i].speed *4;
			else
				speed = text_roll[i].speed ;
			if(SysTick_Cnt - text_roll[i].disp_timer > speed)//滚动时间到了，开始刷新
			{
				text_roll[i].disp_timer = SysTick_Cnt;
				//显示字符
				memset(disp_buf,0,64);
				memcpy(disp_buf,text_roll[i].disp_text,64);
				if(text_roll[i].roll_cnt*2 + text_roll[i].textlen < strlen((char*)disp_buf))
					disp_buf[text_roll[i].roll_cnt*2 + text_roll[i].textlen] = 0;
				ui_disp_china_11x12(text_roll[i].x, text_roll[i].y,(char*)clear_buf,text_roll[i].black_color,text_roll[i].text_color);
				ui_disp_china_11x12(text_roll[i].x, text_roll[i].y,(char*)&disp_buf[text_roll[i].roll_cnt*2],text_roll[i].black_color,text_roll[i].text_color);
				if(text_roll[i].roll_cnt < strlen((char*)text_roll[i].disp_text)/2)//滚动的幅度
					text_roll[i].roll_cnt = (text_roll[i].roll_cnt + 1)%32;
				else
					text_roll[i].roll_cnt = 0;
			}
		}
	}
}
/******      ui task       ********/
void ui_task(void)
{
	static app_status_t old_app_status ,cur_app_status ;
	cur_app_status = application_get_status();
	
	//检测中文滚动显示
	ui_disp_chinese_roll_task();
	if(old_app_status != cur_app_status)
		ui_disp_chinese_roll_stop();	
	
//启动  UI	
	if(cur_app_status == APP_STATUS_STARTING)
	{
		if(old_app_status != cur_app_status)
		{
			lcd_disp_bmp_SingleByte(0,0,logoImg,160, 128);	
			LCD_BK_ON();
			
		}
		
	}
	
//进入POC  	
	else if(cur_app_status == APP_STATUS_ENTER_POC)
	{
		static uint8_t i = 0;
		char seach[7] = "......";			
		static uint32_t change_timer;
		if(old_app_status != cur_app_status)
		{		
			ui_menu_enter_poc();	
			change_timer = SysTick_Cnt;
		}	
		seach[i%6+1] = 0;
		if(SysTick_Cnt - change_timer > 1500)
		{		
			change_timer = SysTick_Cnt;
			ui_disp_china_15x16(16,50,"    搜索网络    ", COLOR_WIGHT, COLOR_BLUE);				
			ui_disp_china_15x16(42+(i++%6)*8,68,"        ", COLOR_WIGHT, COLOR_BLUE);				
			ui_disp_china_15x16(42,68,seach, COLOR_WIGHT, COLOR_BLUE);	
		}				
	}
	
	else if( cur_app_status == APP_STATUS_ENTER_SIM_ERROR)
	{
		if(old_app_status != cur_app_status)
		{		
			ui_disp_china_15x16(55,80,"请插卡：", COLOR_WIGHT, COLOR_RED);	
		}			
	}
//主界面  working状态	
	else if(cur_app_status == APP_STATUS_WORKING_MAIN)
	{
		if(old_app_status != cur_app_status)
			ui_menu_main();	
	
		ui_disp_ican_date(DISENABLE);  //刷新日期
		ui_disp_poc_talk_group();//群组内讲话通知图标		
		ui_disp_poc_talk_pttkey();//按键对讲小喇叭			
		ui_disp_poc_group_refush(0);//群组更新
	}
//设置菜单 主界面
	else if(cur_app_status ==  APP_STATUS_MENU_SET_MAIN)
	{
		if(old_app_status != cur_app_status)
		{
			//刷新主菜单的6个设置图标
			lcd_color_clr(0,35,COLOR_WIGHT,160,78);	
			lcd_disp_bmp_SingleByte(10,43,gImage_qunzu,28, 30);	
			lcd_disp_bmp_SingleByte(70,43,gImage_chengyuan,21, 29);	
			lcd_disp_bmp_SingleByte(124,46,gImage_haoyou,25, 24);
			lcd_disp_bmp_SingleByte(10,80,gImage_jilu,28, 24);
			lcd_disp_bmp_SingleByte(71,80,gImage_gps,18, 26);
			lcd_disp_bmp_SingleByte(123,78,gImage_setup,26, 28);
			lcd_disp_box(0,112,COLOR_BLUE, 160,16); 
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);		
			ui_disp_china_11x12(130,114,"确认", COLOR_BLUE,COLOR_WIGHT);			
			ui_disp_menu_main_select(1);
		}			
		ui_disp_menu_main_select(0);//   选择框刷新    
	}
	
///****   设置菜单， 子菜单1  群组设置   ***/		
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_1)	
	{
		static uint8_t old_select , cur_select, disp_select;	
	  char i,disp_buf[26] = {0} ;
		poc_device_pt poc_para = poc_get_para();
		cur_select = application_get_menu_select();						
		if(old_app_status != cur_app_status)
		{
			lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
			ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);					
			ui_disp_china_15x16(52,18,"群组选择",COLOR_BLUE,COLOR_WIGHT );	
			lcd_color_clr(0,35,COLOR_WIGHT,160,78);
			if(poc_para->group_list_sum >= 1)
				lcd_disp_bmp_SingleByte(17,37,gImage_qunzu22x24,22, 24);	
			if(poc_para->group_list_sum  >= 2)
				lcd_disp_bmp_SingleByte(17,37+25,gImage_qunzu22x24,22, 24);	
			if(poc_para->group_list_sum  >= 3)
				lcd_disp_bmp_SingleByte(17,37+50,gImage_qunzu22x24,22, 24);				
			lcd_disp_box(0,112,COLOR_BLUE, 160,16); 			
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);
			ui_disp_china_11x12(130,114,"确认", COLOR_BLUE,COLOR_WIGHT);			
			old_select = 99;
		}		

		if(old_select != cur_select)
		{
			uint16_t p;
			if(cur_select > old_select && disp_select<2)
				disp_select++;
			else if(cur_select < old_select && disp_select>0)
				disp_select--;	
			
			for(i=0;i<3;i++)//显示群组信息
			{
				uint16_t bk_color = COLOR_WIGHT, text_color = COLOR_BLUE;
				if(i >= poc_para->group_list_sum )break;
				if(cur_select+i+1 >= disp_select)
					p = ((cur_select+i+1 - disp_select)%poc_para->group_list_sum );
				else
					p = poc_para->group_list_sum - disp_select +i+1;
				if(p == 0)p = poc_para->group_list_sum %51;
				
				memset(disp_buf,0,26);
				strHEX_uincode_to_gb2312(poc_para->group_list[p],26,(uint8_t *)disp_buf);
				if(i == disp_select)
				{
					bk_color = COLOR_BLUE;
					text_color = COLOR_WIGHT;
				}
				ui_disp_china_11x12(49,47+i*24,disp_buf, bk_color,text_color);
				lcd_disp_box(49+strlen(disp_buf)*6,47+i*24,bk_color, 154-47-strlen(disp_buf)*6,12);
				lcd_disp_range(47,45+i*24,156-47,16,2,bk_color);
			//填充序列号	
				lcd_disp_box(1,38+i*24,COLOR_BLUE, 15,23);
				ui_disp_itoa(p,6,47+i*24,COLOR_BLUE,COLOR_WIGHT);				
			}
			old_select = cur_select;		
		}		
	}
	
	
	else if(cur_app_status == APP_STATUS_SET_SUBMENU1_2_SEACHING)
	{
		if(old_app_status != cur_app_status)
		{
			lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
			ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);					
			ui_disp_china_15x16(64-26,18,"成员选择",COLOR_BLUE,COLOR_WIGHT );	
			lcd_color_clr(0,36,COLOR_WIGHT,160,110);	
			ui_disp_china_15x16(64-26,68,"请稍后...",COLOR_WIGHT,COLOR_BLUE );	
		}				
	}
///****   设置菜单， 子菜单2——1  成员操作 SETUP设置   ***/			
	else if(cur_app_status == APP_STATUS_SET_SUBMENU1_2_1)
	{
		static uint16_t old_select , cur_select;		
		if(old_app_status != cur_app_status)
		{		
			old_select = 999;
			lcd_disp_box(56+30,50,COLOR_WIGHT, 36,40); 
			lcd_disp_range(56+30,50,36,20,1,COLOR_BLUE);
			lcd_disp_range(56+30,69,36,20,1,COLOR_BLUE);
			lcd_disp_range(56+30,88,36,20,1,COLOR_BLUE);
		}	
    cur_select = application_get_menu_select();
		if(old_select != cur_select)
		{	
		  char disp_msg[3][5] = {"呼叫","好友","取消"},i;
			for(i=0;i<3;i++)
			{
				if(i == application_get_menu_select())
					ui_disp_china_15x16(58+30,52+i*19,disp_msg[i],COLOR_BLUE,COLOR_WIGHT );			
				else
					ui_disp_china_15x16(58+30,52+i*19,disp_msg[i],COLOR_WIGHT,COLOR_BLUE );	
			}		
			old_select = cur_select;	
		}			
	}
///****   设置菜单， 子菜单3——1 好友操作 SETUP设置   ***/			
	else if(cur_app_status == APP_STATUS_SET_SUBMENU1_3_1)
	{
		static uint16_t old_select , cur_select;		
		if(old_app_status != cur_app_status)
		{		
			old_select = 999;
			lcd_disp_box(56+30,50,COLOR_WIGHT, 36,40); 
			lcd_disp_range(56+30,50,36,20,1,COLOR_BLUE);
			lcd_disp_range(56+30,69,36,20,1,COLOR_BLUE);
			lcd_disp_range(56+30,88,36,20,1,COLOR_BLUE);
		}	
    cur_select = application_get_menu_select();
		if(old_select != cur_select)
		{	
		  char disp_msg[3][5] = {"呼叫","删除","取消"},i;
			for(i=0;i<3;i++)
			{
				if(i == application_get_menu_select())
					ui_disp_china_15x16(58+30,52+i*19,disp_msg[i],COLOR_BLUE,COLOR_WIGHT );			
				else
					ui_disp_china_15x16(58+30,52+i*19,disp_msg[i],COLOR_WIGHT,COLOR_BLUE );	
			}		
			old_select = cur_select;	
		}			
	}	
	
	
///****   设置菜单， 子菜单2  成员 SETUP设置   ***/		
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_2)	
	{
		static uint16_t old_select , cur_select, disp_select;	
		char i,disp_buf[26] = {0};
		poc_device_pt poc_para = poc_get_para();
		cur_select = application_get_menu_select();		
	
		if(old_app_status != cur_app_status)
		{
			lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
			ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);					
			ui_disp_china_15x16(52,18,"成员选择",COLOR_BLUE,COLOR_WIGHT );	
			lcd_color_clr(0,35,COLOR_WIGHT,160,78);	
			if(poc_para->member_online_cnt == 0)
				ui_disp_china_15x16(60,70,"无成员",COLOR_WIGHT, COLOR_BLUE);		
			if(poc_para->member_online_cnt  >= 1)
				lcd_disp_bmp_SingleByte(21,37,gImage_chengyuan17X24,17, 24);	
			if(poc_para->member_online_cnt  >= 2)
				lcd_disp_bmp_SingleByte(21,37+25,gImage_chengyuan17X24,17, 24);	
			if(poc_para->member_online_cnt  >= 3)
				lcd_disp_bmp_SingleByte(21,37+50,gImage_chengyuan17X24,17, 24);		
			lcd_disp_box(0,112,COLOR_BLUE, 160,16); 			
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);
			ui_disp_china_11x12(130,114,"操作", COLOR_BLUE,COLOR_WIGHT);			
			old_select = 999;
		}		
	 
		if(old_select != cur_select)
		{
			uint16_t p;
			if(cur_select > old_select && disp_select<2)
				disp_select++;
			else if(cur_select < old_select && disp_select>0)
				disp_select--;	
			
			for(i=0;i<3;i++)
			{
				uint16_t bk_color = COLOR_WIGHT, text_color = COLOR_BLUE;
				if(i >= poc_para->member_online_cnt) break;
				if(cur_select+i+1 >= disp_select)
					p = ((cur_select+i+1 - disp_select)%poc_para->member_online_cnt );
				else
					p = poc_para->member_online_cnt - disp_select +i+1;
				if(p == 0)p = poc_para->member_online_cnt ;
				
				memset(disp_buf,0,26);
				strHEX_uincode_to_gb2312(poc_para->member_list[p],26,(uint8_t *)disp_buf);
				if(i == disp_select)
				{
					bk_color = COLOR_BLUE;
					text_color = COLOR_WIGHT;
				}
				if(0 == memcmp(poc_para->user_name ,disp_buf,strlen(disp_buf)))
					ui_disp_china_11x12(49,47+i*24,disp_buf, bk_color,COLOR_RED);
				else
					ui_disp_china_11x12(49,47+i*24,disp_buf, bk_color,text_color);
				lcd_disp_box(49+strlen(disp_buf)*6,47+i*24,bk_color, 154-47-strlen(disp_buf)*6,12);
				lcd_disp_range(47,45+i*24,156-47,16,2,bk_color);
			//填充序列号	
				lcd_disp_box(1,38+i*24,COLOR_BLUE, 19,23);
				if(poc_para->member_list_status[p] == '3')
					ui_disp_itoa(p,8,47+i*24,COLOR_BLUE,COLOR_WIGHT);	
				else				
					ui_disp_itoa(p,8,47+i*24,COLOR_BLUE,COLOR_RED);				
			}
			old_select = cur_select;		
		}			
			
	}	
	
	
///****   设置菜单， 子菜单3  好友  SETUP设置   ***/		
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_3)	
	{
		static uint16_t old_select , cur_select, disp_select;	
		char i,disp_buf[26] = {0};

		cur_select = application_get_menu_select();		
	
		if(old_app_status != cur_app_status)
		{
			lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
			ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);					
			ui_disp_china_15x16(52,18,"好友选择",COLOR_BLUE,COLOR_WIGHT );	
			lcd_color_clr(0,35,COLOR_WIGHT,160,78);	
			if(friend_read_cnt() == 0 || friend_read_cnt() > 500)
				ui_disp_china_15x16(60,70,"无好友",COLOR_WIGHT, COLOR_BLUE);		
			if(friend_read_cnt() >= 1)
				lcd_disp_bmp_SingleByte(21,37,gImage_chengyuan17X24,17, 24);	
			if(friend_read_cnt() >= 2)
				lcd_disp_bmp_SingleByte(21,37+25,gImage_chengyuan17X24,17, 24);	
			if(friend_read_cnt() >= 3)
				lcd_disp_bmp_SingleByte(21,37+50,gImage_chengyuan17X24,17, 24);		
			lcd_disp_box(0,112,COLOR_BLUE, 160,16); 			
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);
			ui_disp_china_11x12(130,114,"操作", COLOR_BLUE,COLOR_WIGHT);			
			old_select = 999;
		}		
	 
		if(old_select != cur_select)
		{
			uint16_t p;
			if(cur_select > old_select && disp_select<2)
				disp_select++;
			else if(cur_select < old_select && disp_select>0)
				disp_select--;	
			
			for(i=0;i<3;i++)
			{
				uint16_t bk_color = COLOR_WIGHT, text_color = COLOR_BLUE;
				friend_t fd;
				if(i >= friend_read_cnt())break;
				if(cur_select+i+1 >= disp_select)
					p = ((cur_select+i+1 - disp_select)%friend_read_cnt());
				else
					p = friend_read_cnt()- disp_select +i+1;
				if(p == 0)p = friend_read_cnt();
				
				memset(disp_buf,0,26);
				friend_read(p-1,&fd);
				strHEX_uincode_to_gb2312(fd.friend_name,26,(uint8_t *)disp_buf);
				if(i == disp_select)
				{
					bk_color = COLOR_BLUE;
					text_color = COLOR_WIGHT;
				}
				ui_disp_china_11x12(49,47+i*24,disp_buf, bk_color,text_color);
				lcd_disp_box(49+strlen(disp_buf)*6,47+i*24,bk_color, 154-47-strlen(disp_buf)*6,12);
				lcd_disp_range(47,45+i*24,156-47,16,2,bk_color);
			//填充序列号	
				lcd_disp_box(1,38+i*24,COLOR_BLUE, 19,23);	
				ui_disp_itoa(p,8,47+i*24,COLOR_BLUE,COLOR_WIGHT);					
			}
			old_select = cur_select;		
		}				
				
	}
	
	
///****   设置菜单， 子菜单4  记录  SETUP设置   ***/		
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_4)	
	{
		static uint16_t old_select , cur_select, disp_select;	
		char i,disp_buf[26] = {0};

		cur_select = application_get_menu_select();		
	
		if(old_app_status != cur_app_status)
		{
			lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
			ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);					
			ui_disp_china_15x16(52,18,"单呼记录",COLOR_BLUE,COLOR_WIGHT );	
			lcd_color_clr(0,35,COLOR_WIGHT,160,78);	
			if(call_log_read_cnt() == 0 || call_log_read_cnt() > 64)
				ui_disp_china_15x16(60,70,"无记录",COLOR_WIGHT, COLOR_BLUE);		
			if(call_log_read_cnt() >= 1)
				lcd_disp_bmp_SingleByte(17,37,gImage_chengyuan17X24,17, 24);	
			if(call_log_read_cnt() >= 2)
				lcd_disp_bmp_SingleByte(17,37+25,gImage_chengyuan17X24,17, 24);	
			if(call_log_read_cnt() >= 3)
				lcd_disp_bmp_SingleByte(17,37+50,gImage_chengyuan17X24,17, 24);		
			lcd_disp_box(0,112,COLOR_BLUE, 160,16); 			
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);
			ui_disp_china_11x12(130,114,"操作", COLOR_BLUE,COLOR_WIGHT);			
			old_select = 999;
		}		
	 
		if(old_select != cur_select)
		{
			uint16_t p;
			if(cur_select > old_select && disp_select<2)
				disp_select++;
			else if(cur_select < old_select && disp_select>0)
				disp_select--;	
			ui_disp_chinese_roll_stop();
			for(i=0;i<3;i++)
			{
				uint16_t bk_color = COLOR_WIGHT, text_color = COLOR_BLUE;
				call_log_t log;
				uint8_t time_str[6] ;				
				if(i >= call_log_read_cnt())break;
				if(cur_select+i+1 >= disp_select)
					p = ((cur_select+i+1 - disp_select)%call_log_read_cnt());
				else
					p = call_log_read_cnt()- disp_select +i+1;
				if(p == 0)p = call_log_read_cnt();
				
				memset(disp_buf,0,26);
				call_log_read(p,&log);
				strHEX_uincode_to_gb2312(log.caller_name,26,(uint8_t *)disp_buf);
				hex_to_str(&time_str[0],&log.time.RTC_Hours,1);
				time_str[2] = ':';
				hex_to_str(&time_str[3],&log.time.RTC_Minutes,1);

				if(i == disp_select)
				{
					bk_color = COLOR_BLUE;
					text_color = COLOR_WIGHT;
				}
				if(strlen((char*)disp_buf) > 14)
				{
					ui_disp_chinese_roll(CHINESE_TYPE_11x12, 41,47+i*24,disp_buf, bk_color,text_color,14,500);
					lcd_disp_box(41+146,47+i*24,bk_color, 126-42-14*6,12);
					lcd_disp_range(39,45+i*24,128-40,16,2,bk_color);					
				}
				else		
				{
					ui_disp_china_11x12(41,47+i*24,disp_buf, bk_color,text_color);
					lcd_disp_box(41+strlen(disp_buf)*6,47+i*24,bk_color, 126-42-strlen(disp_buf)*6,12);
					lcd_disp_range(39,45+i*24,128-40,16,2,bk_color);
				}		
				ui_disp_china_11x12(130,47+i*24,(char*)time_str, bk_color,text_color);				
				lcd_disp_range(128,45+i*24,33,16,2,bk_color);				
			//填充序列号	
				lcd_disp_box(1,38+i*24,COLOR_BLUE, 15,23);	
				ui_disp_itoa(p,6,47+i*24,COLOR_BLUE,COLOR_WIGHT);				
			}
			old_select = cur_select;		
		}			
	}	
	
///****   写码模式  ***/			
	else if(cur_app_status == APP_STATUS_WRITE_CODE)	
	{
		static uint8_t i = 0;
		char seach[7] = "......";			
		static uint32_t change_timer;
		static uint8_t code_status;
		if(old_app_status != cur_app_status)
		{		
			ui_disp_clr(COLOR_WIGHT);
			lcd_disp_box(0,0,COLOR_BLACK, 160,16); //显示黑块 16*160
			lcd_disp_tan(2,2,12,2,7,7,1,COLOR_WIGHT);//显示三角形
			lcd_disp_box(6,7,COLOR_WIGHT, 2,6); //显示三角形下面的竖线			
			ui_disp_ican_signal();
			ui_disp_ican_bat();			
			
			ui_disp_china_15x16(16,50,"    写码模式    ", COLOR_WIGHT, COLOR_BLUE);				
			change_timer = SysTick_Cnt;
		}	
		if(write_code_get_status() == WRITE_CODE_NOMAL)
		{
			seach[i%6+1] = 0;
			if(SysTick_Cnt - change_timer > 1000)
			{		
				change_timer = SysTick_Cnt;
				ui_disp_china_15x16(42+(i++%6)*8,68,"        ", COLOR_WIGHT, COLOR_BLUE);				
				ui_disp_china_15x16(42,68,seach, COLOR_WIGHT, COLOR_BLUE);	
			}		
			code_status = WRITE_CODE_NOMAL;
		}
		else if(write_code_get_status() == WRITE_CODE_READ_CFG)
		{
			if(code_status != WRITE_CODE_READ_CFG)
			{
				device_cfg_pt device_config = device_config_read(); 
				lcd_disp_box(0,25,COLOR_WIGHT, 160,100); 
				ui_disp_china_11x12(5,25,"账号：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(40,25,(char*)device_config->user_id, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(5,45,"主机：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(40,45,(char*)device_config->server_ip, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(5,65,"APN_ID：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(50,65,(char*)device_config->apn_name, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(5,85,"APN_PWD：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(55,85,(char*)device_config->apn_pwd, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_15x16(60,105,"查询成功！", COLOR_WIGHT,COLOR_BLUE);	
			}
			code_status = WRITE_CODE_READ_CFG;
		}
		else if(write_code_get_status() ==  WRITE_CODE_WRITE_CFG)
		{
			if(code_status != WRITE_CODE_WRITE_CFG)
			{
				device_cfg_pt device_config = device_config_read(); 
				lcd_disp_box(0,25,COLOR_WIGHT, 160,100); 
				ui_disp_china_11x12(5,25,"账号：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(40,25,(char*)device_config->user_id, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(5,45,"主机：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(40,45,(char*)device_config->server_ip, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(5,65,"APN_ID：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(50,65,(char*)device_config->apn_name, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(5,85,"APN_PWD：", COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_11x12(55,85,(char*)device_config->apn_pwd, COLOR_WIGHT,COLOR_BLUE);	
				ui_disp_china_15x16(60,105,"设置成功！", COLOR_WIGHT,COLOR_BLUE);	
			}			
			code_status = WRITE_CODE_WRITE_CFG;
		}
	}
	
///****   设置菜单， 子菜单3——1 好友操作 SETUP设置   ***/			
	else if(cur_app_status == APP_STATUS_SET_SUBMENU1_4_1)
	{
		static uint16_t old_select , cur_select;		
		if(old_app_status != cur_app_status)
		{		
			old_select = 999;
			lcd_disp_box(56+30,50,COLOR_WIGHT, 36,40); 
			lcd_disp_range(56+30,50,36,20,1,COLOR_BLUE);
			lcd_disp_range(56+30,69,36,20,1,COLOR_BLUE);
			lcd_disp_range(56+30,88,36,20,1,COLOR_BLUE);
		}	
    cur_select = application_get_menu_select();
		if(old_select != cur_select)
		{	
		  char disp_msg[3][5] = {"呼叫","删除","取消"},i;
			for(i=0;i<3;i++)
			{
				if(i == application_get_menu_select())
					ui_disp_china_15x16(58+30,52+i*19,disp_msg[i],COLOR_BLUE,COLOR_WIGHT );			
				else
					ui_disp_china_15x16(58+30,52+i*19,disp_msg[i],COLOR_WIGHT,COLOR_BLUE );	
			}		
			old_select = cur_select;	
		}			
	}	
///****   关机     ***/			
	else if(cur_app_status == APP_STATUS_DEVICE_SHUTDOWN)		
	{
		if(old_app_status != cur_app_status)
		{		
			lcd_disp_box(0,0,COLOR_WIGHT, 160,128); 
			ui_disp_china_15x16(46,52,"正在关机",COLOR_WIGHT,COLOR_BLUE );		
		}			
	}
	
///****   设置菜单， 子菜单5  GPS     ***/		
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_5)
	{
		uint8_t i;		
		const char menu_list[4][10] = {	"纬    度:",
																		"经    度:",
																		"已连卫星:",
																		"信号强度:"};
		
		if(old_app_status != cur_app_status)
		{
			lcd_color_clr(0,36,COLOR_WIGHT,160,110);	
			for(i=0;i<4;i++)
				ui_disp_china_11x12(5,40+i*16,(char*)menu_list[i], COLOR_WIGHT,COLOR_BLUE);	
			
			lcd_color_clr(0,113,COLOR_BLUE,160,15);	
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);	
			ui_disp_gps_info(1);
		}		
		//更新 gps数据到显示屏
	  ui_disp_gps_info(0);
		
	}

///****   设置菜单， 子菜单6  版本   ***/	
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_6_2)	
	{
		device_cfg_pt device_config = device_config_read(); 
		if(old_app_status != cur_app_status)
		{
			lcd_color_clr(0,36,COLOR_BABY_BLUE1,160,110);	
			ui_disp_china_11x12(5,40,"账号：", COLOR_BABY_BLUE1,COLOR_BLUE);	
			ui_disp_china_11x12(40,40,(char*)device_config->user_id, COLOR_BABY_BLUE1,COLOR_BLUE);	
			ui_disp_china_11x12(5,60,"主机：", COLOR_BABY_BLUE1,COLOR_BLUE);	
			ui_disp_china_11x12(40,60,(char*)device_config->server_ip, COLOR_BABY_BLUE1,COLOR_BLUE);	
			ui_disp_china_11x12(5,80,"版本：", COLOR_BABY_BLUE1,COLOR_BLUE);	
			ui_disp_china_11x12(40,80,(char*)device_config->ver, COLOR_BABY_BLUE1,COLOR_BLUE);	
			lcd_color_clr(0,113,COLOR_BLUE,160,15);	
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);		
		}			
		
	}		
	
///****   设置菜单， 子菜单6   恢复出厂   ***/	
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_6_3)	
	{
		if(old_app_status != cur_app_status)
		{
			lcd_color_clr(0,36,COLOR_BABY_BLUE1,160,110);	
			ui_disp_china_11x12(40,60,"是否确认", COLOR_BABY_BLUE1,COLOR_BLUE);	
			ui_disp_china_11x12(30,80,"恢复出厂设置？", COLOR_BABY_BLUE1,COLOR_BLUE);	
			lcd_color_clr(0,113,COLOR_BLUE,160,15);				
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);		
			ui_disp_china_11x12(130,114,"确认", COLOR_BLUE,COLOR_WIGHT);		
		}			
		
	}		
	
	
///****   设置菜单， 子菜单6   按键音 ptt音 ***/	
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_6_5 || cur_app_status ==  APP_STATUS_SET_SUBMENU1_6_4)	
	{
		static uint8_t old_select , cur_select;		
		device_cfg_pt device_config = device_config_read();
		uint8_t i;	
		char cnt[2] = "1";		
		const char menu_list[3][18] = {	"打开  ",
																		"关闭  "};				
		if(old_app_status != cur_app_status)
		{
			old_select = 22;
			lcd_color_clr(0,36,COLOR_BABY_BLUE1,160,77);	
			for(i=0;i<2;i++)
			{
				cnt[0] = '1'+i;
				ui_disp_china_15x16(11,40+i*26,cnt, COLOR_BABY_BLUE1,COLOR_BLUE);	
				lcd_disp_range(10-4,40+i*26 , 18,18,1,COLOR_BLUE );					
			}			
			lcd_color_clr(0,113,COLOR_BLUE,160,15);		
      if( (cur_app_status ==  APP_STATUS_SET_SUBMENU1_6_4 && device_config->ptt_sound) || 
				  (cur_app_status ==  APP_STATUS_SET_SUBMENU1_6_5 && device_config->key_sound)  )
				lcd_disp_bit1bmp_SingleByte(80,44,right_dat,12,12,COLOR_RED,COLOR_BABY_BLUE1);
			else 
				lcd_disp_bit1bmp_SingleByte(80,70,right_dat,12,12,COLOR_RED,COLOR_BABY_BLUE1);	

			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);		
			ui_disp_china_11x12(130,114,"确认", COLOR_BLUE,COLOR_WIGHT);		
		}			
		cur_select = application_get_menu_select();	
		if(old_select != cur_select)
		{
			for(i=0;i<2;i++)
			{
				if(cur_select != i)
					ui_disp_china_15x16(28,41+i*26,( char*)menu_list[i], COLOR_BABY_BLUE1,COLOR_BLUE);	
				else
					ui_disp_china_15x16(28,41+i*26,( char*)menu_list[i], COLOR_BLUE,COLOR_BABY_BLUE1);	
			}	
			old_select = cur_select;		
		}			
		
	}		
			
	
///****   设置菜单， 子菜单6   屏幕设置   ***/	
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_6_1)	
	{
		static uint8_t old_select , cur_select;	
		uint8_t i;		
		device_cfg_pt cfg = device_config_read();
		const char menu_list[4][18] = {	"30秒 ",
																		"60秒 ",
																		"常开 ",
																		"自动 "};		
		if(old_app_status != cur_app_status)
		{
			old_select = 222;
			lcd_color_clr(0,36,COLOR_BABY_BLUE1,160,77);
      if( cfg->lcd_sleep_timer == 30000)
				lcd_disp_bit1bmp_SingleByte(60,41,right_dat,12,12,COLOR_RED,COLOR_BABY_BLUE1);			
			else  if(cfg->lcd_sleep_timer  == 60000)
				lcd_disp_bit1bmp_SingleByte(60,60,right_dat,12,12,COLOR_RED,COLOR_BABY_BLUE1);		
			else  if(cfg->lcd_sleep_timer  == 21000)
				lcd_disp_bit1bmp_SingleByte(60,98,right_dat,12,12,COLOR_RED,COLOR_BABY_BLUE1);		
			else
				lcd_disp_bit1bmp_SingleByte(60,79,right_dat,12,12,COLOR_RED,COLOR_BABY_BLUE1);		
			
		}		
	  cur_select = application_get_menu_select();				
		if(old_select != cur_select)
		{
			for(i=0;i<4;i++)
			{
				uint16_t bk_color = COLOR_BABY_BLUE1, text_color = COLOR_BLUE;				
				if(i == cur_select)
				{
					bk_color = COLOR_BLUE;
					text_color = COLOR_BABY_BLUE1;
				}
				ui_disp_china_11x12(26,41+i*19,( char*)menu_list[i], bk_color,text_color);			
				lcd_disp_range(24,39+i*19,32,16,2,bk_color);				
				lcd_disp_range(5,38+i*19 , 16,16,1,COLOR_BLUE );	
				ui_disp_itoa(i+1,10,40+i*19,COLOR_BABY_BLUE1,COLOR_BLUE);				
			}
			old_select = cur_select;									
		}
		
	}		
		
	
///****   设置菜单， 子菜单6   SETUP设置   ***/	
	else if(cur_app_status ==  APP_STATUS_SET_SUBMENU1_6)
	{
		static uint8_t old_select , cur_select, disp_select;	
		uint8_t i;		
		const char menu_list[5][18] = {	"背光设置  ",
																		"关于本机  ",
																		"恢复出厂  ",
																		"PTT音设置 ",
		                                "按键音    "};		
		if(old_app_status != cur_app_status)
		{
			old_select = 222;
			disp_select = 0;
			lcd_color_clr(0,36,COLOR_BABY_BLUE1,160,77);	
			lcd_color_clr(0,113,COLOR_BLUE,160,15);				
			ui_disp_china_11x12(5,114,"返回", COLOR_BLUE,COLOR_WIGHT);		
			ui_disp_china_11x12(130,114,"确认", COLOR_BLUE,COLOR_WIGHT);			
		}		
	  cur_select = application_get_menu_select();				
		if(old_select != cur_select)
		{
			uint16_t p;
			if(cur_select > old_select && disp_select<3)
				disp_select++;
			else if(cur_select < old_select && disp_select>0)
				disp_select--;	
			
			for(i=0;i<4;i++)
			{
				uint16_t bk_color = COLOR_BABY_BLUE1, text_color = COLOR_BLUE;				
				if(cur_select+i+1 >= disp_select)
					p = ((cur_select+i+1 - disp_select)%5);
				else
					p = 5- disp_select +i+1;
				if(p == 0)p = 5;
				
				if(i == disp_select)
				{
					bk_color = COLOR_BLUE;
					text_color = COLOR_BABY_BLUE1;
				}
				ui_disp_china_11x12(26,41+i*19,( char*)menu_list[p-1], bk_color,text_color);			
				lcd_disp_range(24,39+i*19,64,16,2,bk_color);				
				lcd_disp_range(5,38+i*19 , 16,16,1,COLOR_BLUE );	
				ui_disp_itoa(p,10,40+i*19,COLOR_BABY_BLUE1,COLOR_BLUE);				
			}
			old_select = cur_select;									
		}
	}	
	
	
//呼叫主叫界面、被叫界面
	else if(cur_app_status == APP_STATUS_CALLING ||
		      cur_app_status == APP_STATUS_BE_CALLED)
	{
		poc_device_pt poc_para = poc_get_para();
		if(old_app_status != cur_app_status)
		{	
			uint8_t disp_buf[POC_NAME_LENTH_MAX+2] = {0};		
			lcd_disp_box(0,17,COLOR_BLUE, 160,2); 
			ui_disp_china_15x16(0,18,"<                  >", COLOR_BLUE,COLOR_WIGHT);					
			ui_disp_china_15x16(64,18,"单呼",COLOR_BLUE,COLOR_WIGHT );	
			lcd_color_clr(0,36,COLOR_WIGHT,160,78);
			lcd_disp_bit1bmp_SingleByte(30,60,gImage_user,20, 20,COLOR_GREEN,COLOR_WIGHT);	
			lcd_disp_bit1bmp_SingleByte(75,63,gImage_jiantou,13, 13,COLOR_GREEN,COLOR_WIGHT);	
			lcd_disp_bmp_SingleByte(110,60,gImage_chengyuan2,20, 20);	
			if(strlen((char*)poc_para->user_name) > 12)
				ui_disp_chinese_roll(CHINESE_TYPE_11x12,120-12*3,90,(char*)poc_para->user_name, COLOR_WIGHT,COLOR_BLUE,12,500);
			else
				ui_disp_china_11x12(120 - strlen((char*)poc_para->user_name)*3,90,(char*)poc_para->user_name, COLOR_WIGHT,COLOR_BLUE);
			
			strHEX_uincode_to_gb2312(poc_para->call_name,POC_NAME_LENTH_MAX,disp_buf);
			if(strlen((char*)disp_buf) >= 12)
				ui_disp_chinese_roll(CHINESE_TYPE_11x12,40-12*3,90,(char*)disp_buf, COLOR_WIGHT,COLOR_BLUE,12,500);
			else
				ui_disp_china_11x12(40 - strlen((char*)disp_buf)*3,90,(char*)disp_buf, COLOR_WIGHT,COLOR_BLUE);

			lcd_color_clr(0,113,COLOR_BLUE,160,15);		
			ui_disp_china_11x12(5,114,"取消",COLOR_BLUE, COLOR_WIGHT);	
		}	
		//刷新讲话喇叭
		ui_disp_poc_talk_pttkey();
		ui_disp_poc_call_group();
	}

//锁屏界面
	else if(cur_app_status == APP_STATUS_WORKING_LOCKED)
	{
		if(old_app_status != cur_app_status)
		{	
			if(old_app_status != APP_STATUS_WORKING_UNLOCKING)
			{
				ui_menu_main();
				ui_disp_ican_date(ENABLE);
				ui_disp_china_11x12(5,113,"解锁", COLOR_BLUE,COLOR_WIGHT);
				ui_disp_china_11x12(35,98,"              ", COLOR_WIGHT,COLOR_WIGHT);
			}
			else
				ui_disp_china_11x12(35,98,"              ", COLOR_WIGHT,COLOR_WIGHT);	
		}	
		ui_disp_poc_talk_group();//群组内讲话通知图标	
		ui_disp_poc_talk_pttkey();//按键对讲小喇叭	
		ui_disp_poc_group_refush(0);
	}
	//解锁界面
	else if(cur_app_status == APP_STATUS_WORKING_UNLOCKING)
	{
		if(old_app_status != cur_app_status)
		{		
			ui_disp_china_11x12(35,98,"继续按 * 解锁", COLOR_WIGHT,COLOR_RED);
		}
		ui_disp_poc_talk_group();//群组内讲话通知图标		
		ui_disp_poc_talk_pttkey();//按键对讲小喇叭			
	}	
	
	//检测信号强度，检测电池状态，检测时间，有变化则刷新
	if(cur_app_status != APP_STATUS_STARTING && cur_app_status != APP_STATUS_DEVICE_SHUTDOWN  && cur_app_status != APP_STATUS_NORMAL)
	{
		ui_disp_ican_time();
		ui_disp_ican_signal();
		ui_disp_ican_bat();		
		ui_disp_ican_gps();		
	}
	
	old_app_status = cur_app_status;
}




