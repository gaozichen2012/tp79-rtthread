#ifndef __GT21L16S2_H_
#define __GT21L16S2_H_

#include "stm32f10x.h"
#include "typedef.h"

//SPI

#define ADDR_BASE_15X16   0
#define ADDR_BASE_11X16   0X3CF80
#define ADDR_ASCII_8X16   0X3B7C0
#define ADDR_ASCII_8X12		0x66D40
#define ADDR_UNICODE_TO_GB2312	0x67D70
#define ADDR_GB2312_TO_UNICODE	0x2F00

#define SPI_CS_PORT				GPIOD
#define SPI_SCLK_PORT			GPIOB
#define SPI_MOSI_PORT			GPIOE
#define SPI_MISO_PORT			GPIOB
#define SPI_WPN_PORT			GPIOB


#define SPI_CS_PIN				GPIO_Pin_12
#define SPI_SCLK_PIN			GPIO_Pin_2
#define SPI_MOSI_PIN			GPIO_Pin_7
#define SPI_MISO_PIN			GPIO_Pin_1
#define SPI_WPN_PIN			    GPIO_Pin_14


#define SPI_CS_HIGHT()  GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN)
#define SPI_CS_LOW()  	GPIO_ResetBits(SPI_CS_PORT, SPI_CS_PIN)

#define SPI_CLK_HIGHT()  GPIO_SetBits(SPI_SCLK_PORT, SPI_SCLK_PIN)
#define SPI_CLK_LOW()  	GPIO_ResetBits(SPI_SCLK_PORT, SPI_SCLK_PIN)

#define SPI_MOSI_HIGHT()  GPIO_SetBits(SPI_MOSI_PORT, SPI_MOSI_PIN)
#define SPI_MOSI_LOW()  	GPIO_ResetBits(SPI_MOSI_PORT, SPI_MOSI_PIN)

#define SPI_MISO_HIGHT()  GPIO_SetBits(SPI_MISO_PORT, SPI_MISO_PIN)
#define SPI_MISO_LOW()  	GPIO_ResetBits(SPI_MISO_PORT, SPI_MISO_PIN)
#define SPI_MISO_READ()   GPIO_ReadInputDataBit(SPI_MISO_PORT, SPI_MISO_PIN)



/******************************************************************************
** SPI_FLASH
********************************************************************************/
typedef enum
{
    RET_SUCCESS = 0,
    RET_FlashRead_ERROR,
    RET_WriteCmd_ERROR,
    RET_WaitBusy_ERROR
}SpiRetValueType;
/*****************************************************************************
** 管脚定义  GPIO_ResetBits  GPIO_SetBits
*****************************************************************************/
		
#define    SPIFlash_Enable_CS      GPIO_ResetBits(SPI_CS_PORT,SPI_CS_PIN)           //打开spiflash片选使能
#define    SPIFlash_Disable_CS     GPIO_SetBits(SPI_CS_PORT,SPI_CS_PIN)          //关闭spiflash片选使能
#define    SPIFlash_Set_SCLK       GPIO_SetBits(SPI_SCLK_PORT,SPI_SCLK_PIN)           //拉高spiflash时钟线
#define    SPIFlash_Clr_SCLK       GPIO_ResetBits(SPI_SCLK_PORT,SPI_SCLK_PIN)           //拉低spiflash时钟线

#define    SPIFlash_Set_DO         GPIO_SetBits(SPI_MOSI_PORT,SPI_MOSI_PIN)           //MOSI数据线置1,即输出1高电平
#define    SPIFlash_Clr_DO         GPIO_ResetBits(SPI_MOSI_PORT,SPI_MOSI_PIN)           //MOSI数据线清0,即输出0低电平

#define    SPIFlash_Get_DI         GPIO_ReadInputDataBit(SPI_MISO_PORT,SPI_MISO_PIN)


#define    SPIFLASH_CMD_LENGTH        0x03
#define    SPIFLASH_WRITE_BUSYBIT     0X01
#define    SPIFlash_ReadData_CMD      0x03
#define    SPIFlash_WriteEnable_CMD   0x06
#define    SPIFlash_WriteDisable_CMD  0x04
#define    SPIFlash_PageProgram_CMD   0x02
#define    SPIFlash_WriteSR_CMD       0x01
#define    SPIFlash_ReadSR_CMD        0x05
#define    SPIFlash_SecErase_CMD      		0x20
#define    SPIFlash_32K_BlockErase_CMD    0x52
#define    SPIFlash_64K_BlockErase_CMD    0xD8
#define    SPIFlash_ReadID_CMD        		0xAB
#define    SPIFlash_PAGEBYTE_LENGTH   		256
#define    SPIFlash_SECTOR_SIZE   			4096
#define    SPIFlash_BLOCK_SIZE  			0x8000

#define    SPIFlash_SEC_SIZE   SPIFlash_SECTOR_SIZE






void gt21l16s2_init(void);

//输入一个GB2312汉字，输出15*16点阵数据
uint8_t gt21l16s2_get_gb2312_15x16(uint8_t *out_dat, uint8_t* dat_gb2312);
//输入一个ASCII，输出8*16点阵数据
uint8_t gt21l16s2_get_ascii_8x16(uint8_t *out_dat, uint8_t ascii);
uint8_t gt21l16s2_get_gb2312_11x12(uint8_t *out_dat, uint8_t* dat_gb2312);
uint8_t gt21l16s2_get_ascii_6x12(uint8_t *out_dat, uint8_t ascii);
//输入小端模式数据, uincode转GB2312
uint16_t unicode_to_gb2312(uint16_t UCSData);
//GB2312转Uincode 输出小端模式
uint16_t gb2312_to_unicode(uint16_t GBData);

#endif

