#ifndef __UI_H_
#define __UI_H_


#include "device_config.h"

#ifdef LCD_DEVICE_NV3021
#include "nv3021.h"

#define lcd_color_clr		nv3021_color_clr
#define lcd_init				nv3021_init
#define lcd_disp_range		nv3021_disp_range
#define lcd_disp_box		nv3021_disp_box
#define lcd_disp_bmp_SingleByte	nv3021_disp_bmp_SingleByte
#define lcd_disp_bit1bmp_SingleByte	 nv3021_disp_bit1bmp_SingleByte
#define lcd_disp_tan		nv3021_disp_tan

#endif

#ifdef LCD_DEVICE_GC9106

#include "GC9106D.H"

#define lcd_color_clr		GC9106_color_clr
#define lcd_init				GC9106_init
#define lcd_disp_range		GC9106_disp_range
#define lcd_disp_box		GC9106_disp_box
#define lcd_disp_bmp_SingleByte	GC9106_disp_bmp_SingleByte
#define lcd_disp_bit1bmp_SingleByte	 GC9106_disp_bit1bmp_SingleByte
#define lcd_disp_tan		GC9106_disp_tan

#endif

void ui_task(void);

#endif


