#ifndef __FLASH_H_
#define __FLASH_H_

#define FLASH_SIZE_1K		(1024)   			/* FLASH Page Size 1K*/
#define FLASH_SIZE_2K		(2048)   			/* FLASH Page Size 2K*/


///--------500K 后续字节保留做升级标志
#define FLASH_ADDR_RESERVE    (0x08000000 +FLASH_SIZE_1K*500)          //500K  0X0807D000 - 0k
#define FLASH_ADDR_BOOT_FLAG  (FLASH_ADDR_RESERVE + FLASH_SIZE_2K)    ///       0X0807D000 + 2K
#define FLASH_ADDR_USER_START (FLASH_ADDR_BOOT_FLAG + FLASH_SIZE_2K)  //        0X0807D000 + 4K



void Flash_Erase(uint32_t StartAddr,uint32_t Size);
void Flash_Updata(uint32_t StartAddr,uint8_t * Data,uint32_t Size);
void Flash_Write(uint32_t StartAddr,uint8_t * Data,uint32_t Size);//注意要按4字节对齐
void Flash_Read(uint32_t StartAddr,uint8_t * Buffer, uint32_t Size);
void Flash_Move(uint32_t DecAddr, uint32_t SrcAddr, uint32_t Size);
void Flash_rdp(void);


#endif





