/**--------------------------------------------------------------------------------------------------------
** Created date:        2017-08-21
** Version:             V1.00
** Descriptions:     �޸�pin mapҪע���³�ʼ��gpio���޸�        

������ͨ��TFT NV3020������ DOT 160*132
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


#ifndef __NV3021_H_
#define __NV3021_H_


#include "device_config.h"
#include "stm32f10x.h"

#define  KEWEISANTONG_128160    //������ͨS5���ͣ� 128*160  NV3020

#ifdef KEWEISANTONG_128160

#define LCD_WIDT_LENTH 		160
#define LCD_HEIGHT_LENTH 	128

#define LED_RED_PORT    GPIOB
#define LED_RED_PIN     GPIO_Pin_9

#define LED_GREEN_PORT  GPIOE
#define LED_GREEN_PIN   GPIO_Pin_0




//LCD����������ŵĶ˿ں�pin
#define LCD_BACKLIGHT_CTL_PORT	GPIOB
#define LCD_BACKLIGHT_CTL_PIN	GPIO_Pin_8
#define LCD_BK_OFF()	 GPIO_ResetBits(LCD_BACKLIGHT_CTL_PORT, LCD_BACKLIGHT_CTL_PIN)
#define LCD_BK_ON()  	 GPIO_SetBits(LCD_BACKLIGHT_CTL_PORT, LCD_BACKLIGHT_CTL_PIN)
#define LCD_BK_READ()  	 GPIO_ReadOutputDataBit(LCD_BACKLIGHT_CTL_PORT, LCD_BACKLIGHT_CTL_PIN)

#define	LCD_BK_REVERSE()	if(LCD_BK_READ())	\
								LCD_BK_OFF();	\
							else	\
							LCD_BK_ON();	\


//LCD���ƺ���������
#define LCD_BUS_PORT	GPIOE
#define LCD_RST_PORT	GPIOE
#define LCD_CS_PORT		GPIOE
#define LCD_RS_PORT		GPIOE
#define LCD_WR_PORT     GPIOE
#define LCD_RD_PORT		GPIOE
							
#define LCD_DATA_PORT	GPIOE
							
#define LCD_RST_PIN		GPIO_Pin_6
#define LCD_CS_PIN		GPIO_Pin_2
#define LCD_RS_PIN		GPIO_Pin_5
#define LCD_WR_PIN		GPIO_Pin_3
#define LCD_RD_PIN		GPIO_Pin_4
							
#define LCD_DATA_PIN	 GPIO_Pin_8		\
						|GPIO_Pin_9 	\
						|GPIO_Pin_10	\
						|GPIO_Pin_11	\
						|GPIO_Pin_12	\
						|GPIO_Pin_13	\
						|GPIO_Pin_14	\
						|GPIO_Pin_15

						
#define LCD_CS_HIGHT()  GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN)
#define LCD_CS_LOW()  	GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN)
#define LCD_RS_HIGHT()  GPIO_SetBits(LCD_WR_PORT, LCD_RS_PIN)
#define LCD_RS_LOW()  	GPIO_ResetBits(LCD_WR_PORT, LCD_RS_PIN)
#define LCD_REST_OFF()	GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN)
#define LCD_REST_ON()  	GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN)
#define	LCD_WR_LOW()  	GPIO_ResetBits(LCD_WR_PORT, LCD_WR_PIN)
#define	LCD_WR_HIGHT()	GPIO_SetBits(LCD_WR_PORT, LCD_WR_PIN)
#define	LCD_RD_LOW()  	GPIO_ResetBits(LCD_RD_PORT, LCD_RD_PIN)
#define	LCD_RD_HIGHT()	GPIO_SetBits(LCD_RD_PORT, LCD_RD_PIN)

#define	LCD_SEND_DAT8(x)	LCD_DATA_PORT->ODR=(LCD_DATA_PORT->ODR&0x00ff)|(((x)<<8)&0xff00)
						
						
											
#define LCD_DATA_PIN_MASK		0xF0		//GPIO8~GPIO15

#define LCD_SIZE_WIDTH	160
#define LCD_SIZE_HEIGHT	128
#define COLOR_RED		0xF800			
#define COLOR_WIGHT		0xFFFF				
#define COLOR_BLACK		0x0000						
#define COLOR_GREEN		0x07E0						
#define COLOR_BLUE		0x001F			 
#define COLOR_BABY_BLUE1	0xEFBF
#define COLOR_DARK_BLUE	0x79FF
#define COLOR_YELLOW	0xFFE0			   
#define COLOR_GRAY		0x8410				

void ILI9163C_Initial(void);

void nv3021_init(void);
void nv3021_display_turnon (void);
void nv3021_display_turnoff (void);
void nv3021_sleep_in (void);
void nv3021_sleep_out (void);


void nv3021_color_clr(uint8_t x,uint8_t y, uint16_t colorData, uint8_t width, uint8_t height);	//�����
void nv3021_set_dis_range(uint16_t x,uint16_t y,uint16_t m,uint16_t k);  //������ʾ����
#define nv3021_disp_box nv3021_color_clr
void nv3021_disp_bmp_DoubleByte(uint8_t x,uint8_t y,const uint16_t *bmp,uint8_t width, uint8_t height);//��ʾͼƬ˫�ֽ�����
void nv3021_disp_bmp_SingleByte(uint8_t x,uint8_t y,const uint8_t *bmp,uint8_t width, uint8_t height);//��ʾͼƬ���ֽ�����
//���뵥ɫλͼ���ݣ������ɫ�뱳��ɫ��ʾ
//����ȡģ����ֱ8λ����
void nv3021_disp_bit1bmp_SingleByte(uint8_t x,uint8_t y,const uint8_t *bmp,uint8_t width, uint8_t height,uint16_t colorData,uint16_t back_color);
void nv3021_disp_dot(uint16_t StartX,uint16_t StartY,uint16_t Color); //д��
void nv3021_disp_line(uint16_t StartX,uint16_t StartY, uint16_t EndX,uint16_t EndY, uint16_t Width,uint16_t Color); //����
void nv3021_disp_tan(uint16_t X0,uint16_t Y0,uint16_t X1,uint16_t Y1,uint16_t X2,uint16_t Y2,uint16_t LineWidth,uint16_t Color); //�������ο�
void nv3021_disp_range(uint16_t x, uint16_t y, uint16_t width,uint16_t hight,uint8_t LineWidth,uint16_t Color);	//������ 
#endif



#endif


