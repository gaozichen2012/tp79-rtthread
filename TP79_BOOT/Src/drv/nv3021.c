/**--------------------------------------------------------------------------------------------------------
** Created by:         pengfei li
** Created date:        2017-08-21
** Version:             V1.00
** Descriptions:             

科威三通：TFT NU3020驱动， DOT 160*132
CS  |  PB0
RST |  PB1
BLK |  PB2 
CS  |  PB3
RST |  PB4
RS  |  PB5
WR  |  PB6
RD  |  PB7
DAT  PB8~PB15
--------------------------------------------------------------------------------------------------------**/


#include "stm32f10x.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "nv3021.h"
#include "typedef.h"

 void Delay_Ms(uint32_t ms)
{
	uint32_t i,j;
	for(j=0;j<ms;j++)
	{
		for(i=0;i<6050;i++);//
	}

}

static void lcd_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	///---------液晶端口初始化
	GPIO_PIN Lcd_Pins =  	 LCD_RST_PIN	\
							|LCD_CS_PIN		\
							|LCD_RS_PIN		\
							|LCD_WR_PIN		\
							|LCD_RD_PIN		\
							|LCD_DATA_PIN;

	GPIO_InitStructure.GPIO_Pin =  Lcd_Pins;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_BUS_PORT, &GPIO_InitStructure);

	///----------背光灯初始化
	GPIO_InitStructure.GPIO_Pin =  LCD_BACKLIGHT_CTL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_BACKLIGHT_CTL_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LCD_BACKLIGHT_CTL_PORT, LCD_BACKLIGHT_CTL_PIN);

	///----------指示LED----------
	GPIO_InitStructure.GPIO_Pin =  LED_GREEN_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);
	
	GPIO_InitStructure.GPIO_Pin =  LED_RED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);

	
}

void Lcd_Reset(void)
{	
	LCD_REST_ON();
	Delay_Ms(500);
	LCD_REST_OFF();
	Delay_Ms(500);
}

//8080时序
void lcd_write_cmd8(uint8_t cmd) 
{
	LCD_RD_HIGHT();  //禁止读
	LCD_CS_LOW();  //使能
	LCD_RS_LOW(); //写命令
	LCD_WR_LOW();     //写时序	
	LCD_SEND_DAT8(cmd);  //发送数据
	LCD_WR_HIGHT(); 	
	LCD_CS_HIGHT(); 	
}
//8080时序
void lcd_write_dat8(uint8_t Data)
{
	LCD_RD_HIGHT();  //禁止读
	LCD_CS_LOW();  //使能
	LCD_RS_HIGHT(); //写数据
	LCD_SEND_DAT8(Data);  //发送数据
	LCD_WR_LOW();     	  //写时序
	LCD_WR_HIGHT(); 	
	LCD_CS_HIGHT(); 		
}

void lcd_write_dat16(uint16_t Data)
{
	LCD_RD_HIGHT();  //禁止读
	LCD_CS_LOW();  //使能
	LCD_RS_HIGHT(); //写数据
	LCD_SEND_DAT8((Data>>8) & 0x00ff);  //发送数据高8位
	LCD_WR_LOW();     //写时序
	LCD_WR_HIGHT(); 	
	LCD_SEND_DAT8(Data & 0x00ff);  //发送数据低8位	
	LCD_WR_LOW();     //写时序
	LCD_WR_HIGHT(); 		
	LCD_CS_HIGHT(); 	
}




//初始化GPIO  屏的参数
void nv3021_init(void)
{
	lcd_gpio_init();
	Lcd_Reset();
	GPIO_SetBits(LCD_RD_PORT,LCD_RD_PIN);
	
	lcd_write_cmd8(0XFE);
	lcd_write_cmd8(0XEF);	
	lcd_write_cmd8(0x11);  //退出睡眠	
	Delay_Ms(120); //Delay 120ms
	Delay_Ms(120); //Delay 120ms
	lcd_write_cmd8(0XB1);  //帧控制
	lcd_write_dat8(0X05);	
	lcd_write_dat8(0X3c);	
	lcd_write_dat8(0X3c);	
	
	lcd_write_cmd8(0XB2);
	lcd_write_dat8(0X05);	
	lcd_write_dat8(0X3c);	
	lcd_write_dat8(0X3c);
	
	lcd_write_cmd8(0XB3);
	lcd_write_dat8(0X05);	
	lcd_write_dat8(0X3c);	
	lcd_write_dat8(0X3c);	
	lcd_write_dat8(0X05);	
	lcd_write_dat8(0X3c);	
	lcd_write_dat8(0X3c);	
	
	lcd_write_cmd8(0XB4);
	lcd_write_dat8(0X03); 
//-------------------- Power Sequence----------------------------
	lcd_write_cmd8(0xc0);
	lcd_write_dat8(0x28);
	lcd_write_dat8(0x08);
	lcd_write_dat8(0x04);
	
	lcd_write_cmd8(0xc1);
	lcd_write_dat8(0xC0);
	
	lcd_write_cmd8(0xc2);
	lcd_write_dat8(0x0d);
	lcd_write_dat8(0x00);
	
	lcd_write_cmd8(0xc3);
	lcd_write_dat8(0x8d);
	lcd_write_dat8(0x2a);
	
	lcd_write_cmd8(0xc4);
	lcd_write_dat8(0x8D);
	lcd_write_dat8(0xEE);	
	
	lcd_write_cmd8(0xc5);	//vcom control
	lcd_write_dat8(0x14);
	lcd_write_dat8(0x30);	


	lcd_write_cmd8(0x36);	//显示memory数据方向设置 RGB/BGR  MH left/right
	lcd_write_dat8(0xFC); //RGB
	 
	lcd_write_cmd8(0xff);	
	lcd_write_dat8(0x40);
	lcd_write_dat8(0x03);	
	lcd_write_dat8(0x1a);	
	
	lcd_write_cmd8(0xfc);	
	lcd_write_dat8(0x11);
	lcd_write_dat8(0x17);		
	
	lcd_write_cmd8(0x3a);//65k mode
	lcd_write_dat8(0x05);
	Delay_Ms(1);
	lcd_write_cmd8(0x29);	//Display on
	

}


void ILI9163C_Initial(void)
{
	lcd_gpio_init();
	Lcd_Reset();
	GPIO_SetBits(LCD_RD_PORT,LCD_RD_PIN);

	// VCI=2.8V
	//************* Reset LCD Driver ****************//
	GPIO_SetBits(LCD_RST_PORT,LCD_RST_PIN);
	Delay_Ms(1); // Delay 1ms
	GPIO_ResetBits(LCD_RST_PORT,LCD_RST_PIN);
	Delay_Ms(10); // Delay 10ms // This delay time is necessary
	GPIO_SetBits(LCD_RST_PORT,LCD_RST_PIN);
	Delay_Ms(100); // Delay 50 ms
	//************* Start Initial Sequence **********//
	lcd_write_cmd8(0x11); //Exit Sleep
	Delay_Ms(120);

	///lcd_write_cmd8(0x26); //Set Default Gamma
	///lcd_write_dat8(0x04);

	lcd_write_cmd8(0xB1);		///正常模式/全彩数据帧速率
	lcd_write_dat8(0x05); 
	lcd_write_dat8(0x3C); 
	lcd_write_dat8(0x3C);


	lcd_write_cmd8(0xB4);      ///反显控制
	lcd_write_dat8(0x00);

	//------------------------------------ST7735S PowerSequence-----------------------------------------// 

	lcd_write_cmd8(0xC0); 
	lcd_write_dat8(0x28); 
	lcd_write_dat8(0x08); 
	lcd_write_dat8(0x04); 

	lcd_write_cmd8(0xC1); 
	lcd_write_dat8(0XC0); 

	lcd_write_cmd8(0xC2); 
	lcd_write_dat8(0x0D); 
	lcd_write_dat8(0x00); 

	lcd_write_cmd8(0xC3); 
	lcd_write_dat8(0x8D); 
	lcd_write_dat8(0x2A); 

	lcd_write_cmd8(0xC4); 
	lcd_write_dat8(0x8D); 
	lcd_write_dat8(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 

	lcd_write_cmd8(0xC5); //VCOM 电压控制
	lcd_write_dat8(0x1A); 

	//----------------------------------屏幕闪烁问题----------------------------------
	lcd_write_cmd8(0xC7);
	lcd_write_dat8(0xB6); ///0xB6 X , 0x40 X  0x80 X   0xFF  X  0xC4  X

	lcd_write_cmd8(0x3a); //Set Color Format  65K 即RGB565  ，2的16次方
	lcd_write_dat8(0x05);

	lcd_write_cmd8(0x2A); //Set Column Address----横向起始像素和结束像素 160-1
	lcd_write_dat8(0x00);
	lcd_write_dat8(0x00);
	lcd_write_dat8(0x00);
	lcd_write_dat8(0x9F);

	lcd_write_cmd8(0x2B); //Set Page Address----纵向起始像素和结束像素   128-1
	lcd_write_dat8(0x00);
	lcd_write_dat8(0x00);
	lcd_write_dat8(0x00);
	lcd_write_dat8(0x7F);

	lcd_write_cmd8(0x36); //Set Scanning Direction --MX, MY,(水平、垂直扫描顺序)   、  RGB mode (RGB or BGR)
	lcd_write_dat8(0x60);


	///lcd_write_cmd8(0xf2); //Enable Gamma bit
	///lcd_write_dat8(0x01);

	//------------------------------------ST7735S GammaSequence-----------------------------------------// 

	lcd_write_cmd8(0xE0); 
	lcd_write_dat8(0x04); 
	lcd_write_dat8(0x22); 
	lcd_write_dat8(0x07); 
	lcd_write_dat8(0x0A); 
	lcd_write_dat8(0x2E); 
	lcd_write_dat8(0x30); 
	lcd_write_dat8(0x25); 
	lcd_write_dat8(0x2A); 
	lcd_write_dat8(0x28); 
	lcd_write_dat8(0x26); 
	lcd_write_dat8(0x2E); 
	lcd_write_dat8(0x3A); 
	lcd_write_dat8(0x00); 

	lcd_write_dat8(0x01); 
	lcd_write_dat8(0x03); 
	lcd_write_dat8(0x13); 

	lcd_write_cmd8(0xE1); 
	lcd_write_dat8(0x04); 
	lcd_write_dat8(0x16); 
	lcd_write_dat8(0x06); 
	lcd_write_dat8(0x0D); 
	lcd_write_dat8(0x2D); 
	lcd_write_dat8(0x26); 
	lcd_write_dat8(0x23); 
	lcd_write_dat8(0x27); 
	lcd_write_dat8(0x27); 
	lcd_write_dat8(0x25); 
	lcd_write_dat8(0x2D); 
	lcd_write_dat8(0x3B); 
	lcd_write_dat8(0x00); 
	lcd_write_dat8(0x01); 
	lcd_write_dat8(0x04); 
	lcd_write_dat8(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------// 

	Delay_Ms(200);
	lcd_write_cmd8(0x29); // Display On



}


void nv3021_display_turnon (void)
{
	lcd_write_cmd8(0x29);	//Display on
}
void nv3021_display_turnoff (void)
{
	lcd_write_cmd8(0x28);	//Display off
}

void nv3021_sleep_in (void)
{
	lcd_write_cmd8(0x10); //Sleep in
	Delay_Ms(120);
}
void nv3021_sleep_out (void)
{
	lcd_write_cmd8(0x11);	 //Sleep out
	Delay_Ms(120);
}

void nv3021_set_dis_range(uint16_t x,uint16_t y,uint16_t m,uint16_t k)
{
    lcd_write_cmd8(0x2a);    //定义X地址的开始及结束位置
    lcd_write_dat8(x/256);
    lcd_write_dat8(x%256);
    lcd_write_dat8((x+m)/256);	
    lcd_write_dat8((x+m)%256);  
    
    lcd_write_cmd8(0x2b);    //定义Y地址的开始及结束位置
    lcd_write_dat8(y/256);
    lcd_write_dat8(y%256);
    lcd_write_dat8((y+k)/256);	
    lcd_write_dat8((y+k)%256);  
}

//设置底色
void nv3021_color_clr(uint8_t x,uint8_t y, uint16_t colorData, uint8_t width, uint8_t height)
{
	int i,j;
	nv3021_set_dis_range(x,y,width-1,height-1);
	lcd_write_cmd8(0x2c);  	//以下开始数据传输
	for(i=0;i<height;i++)
	{			
		for(j = 0; j < width; j++)
		{
			lcd_write_dat16(colorData);
		}
	}	
}

//显示图片单字节数据
void nv3021_disp_bmp_SingleByte(uint8_t x,uint8_t y,const uint8_t *bmp,uint8_t width, uint8_t height)
{
	uint8_t i,j;
	nv3021_set_dis_range(x,y,width-1,height-1);
	lcd_write_cmd8(0x2c);  	//以下开始数据传输
	
	for(i=0;i<height;i++)
	{			
		for(j = 0; j < width; j++)
		{
			LCD_RD_HIGHT();  //禁止读
			LCD_CS_LOW();  //使能
			LCD_RS_HIGHT(); //写数据
			LCD_SEND_DAT8(*(bmp++));  //发送数据高8位
			LCD_WR_LOW();     //写时序
			LCD_WR_HIGHT(); 	
			LCD_SEND_DAT8(*(bmp++));  //发送数据低8位	
			LCD_WR_LOW();     //写时序
			LCD_WR_HIGHT(); 		
			LCD_CS_HIGHT(); 
		}
	}	
}

//输入单色位图数据，填充颜色与背景色显示
//横向扫描， 垂直8bit数据
//  4*4 点阵    0x08, 0x08,0x08,0x08    ,w=4, h=8
//   0 0 0 0 
//   0 0 0 0
//   0 0 0 0
//   0 0 0 0
//   1 1 1 1
//   0 0 0 0
//   0 0 0 0
void nv3021_disp_bit1bmp_SingleByte(uint8_t x,uint8_t y,const uint8_t *bmp,uint8_t width, uint8_t height,uint16_t colorData,uint16_t back_color)
{
	uint8_t i,j;
	nv3021_set_dis_range(x,y,width-1,height-1);
	lcd_write_cmd8(0x2c);  	//以下开始数据传输

	for(i=0; i<height; i++)
	{			
		for(j=0; j<width; j++)
		{
			if( (*(bmp+j+(i/8)*height)) & (0x01 << (i%8)) )//写colordata
					lcd_write_dat16(colorData);
			else  //写back_color
					lcd_write_dat16(back_color);			
		}
	}	
	
}



//显示图片双字节数据
void nv3021_disp_bmp_DoubleByte(uint8_t x,uint8_t y,const uint16_t *bmp,uint8_t width, uint8_t height)
{
	uint8_t i,j;
	uint16_t temp;
	nv3021_set_dis_range(x,y,width-1,height-1);
	lcd_write_cmd8(0x2c);  	//以下开始数据传输
	
	for(i=0;i<height;i++)
	{			
		for(j = 0; j < width; j++)
		{
			temp = *(bmp+(height-i-1)*width+j);
			temp = (temp >> 8) + (temp << 8);
			lcd_write_dat16(temp);
		}
	}	
}

void nv3021_disp_dot(uint16_t StartX,uint16_t StartY,uint16_t Color) //写点
{
	nv3021_disp_box(StartX,StartY,Color, 1,1);
}


void nv3021_disp_line(uint16_t StartX,uint16_t StartY, uint16_t EndX,uint16_t EndY, uint16_t Width,uint16_t Color)
{
		int y=0,i=0;
		int x0,y0,x1;//,y1;
		if(StartX<EndX)
		{
			x0= StartX;
			x1= EndX;
			y0 = StartY;
			//y1 = EndY;
		}
		else
		{
			x0= EndX;
			x1= StartX;
			y0 = EndY;
			//y1 = StartY;
		}
		for(i=x0;i<=x1;i++)
		{
			y = (int)((((double)(i-x0))/((double)(EndX-StartX))) * ((double)EndY-(double)StartY));
			nv3021_disp_box(i,(uint16_t)(y0+y),Color, 1,Width);
		}
}

void nv3021_disp_tan(uint16_t X0,uint16_t Y0,uint16_t X1,uint16_t Y1,uint16_t X2,uint16_t Y2,uint16_t LineWidth,uint16_t Color)
{
		nv3021_disp_line(X0,Y0,X1-1,Y1,LineWidth,Color);
		nv3021_disp_line(X1,Y1,X2-1,Y2,LineWidth,Color);
		nv3021_disp_line(X2,Y2,X0,Y0,LineWidth,Color);
}

void nv3021_disp_range(uint16_t x, uint16_t y, uint16_t width,uint16_t hight,uint8_t LineWidth,uint16_t Color)
{
	nv3021_disp_box(x,y,Color,width,LineWidth);
	nv3021_disp_box(x,y,Color,LineWidth,hight);
	nv3021_disp_box(x+width-LineWidth,y,Color,LineWidth,hight);
	nv3021_disp_box(x,y+hight-LineWidth,Color,width,LineWidth);
}

