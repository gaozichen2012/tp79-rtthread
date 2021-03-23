#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "flash.h"
#include "string.h"

void Flash_Erase(uint32_t StartAddr,uint32_t Size)
{
	uint8_t i;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	for(i=0;i<Size/0x800;i++)
		FLASH_ErasePage(StartAddr+i*0x800); //擦除该页

	FLASH_Lock();
	
}

void Flash_Write(uint32_t StartAddr,uint8_t * Data,uint32_t Size)
{
	uint32_t i;
	uint32_t Data32;
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	
	for(i=0;i<Size;i+=4)//写数据
	{
		Data32 = (uint32_t)Data[i] + (uint32_t)(Data[i+1] << 8)+ (uint32_t)(Data[i+2] << 16)+ (uint32_t)(Data[i+3] << 24);
		FLASH_ProgramWord( StartAddr+i, Data32);
	}
	
	FLASH_Lock();
}

void Flash_Updata(uint32_t StartAddr,uint8_t * Data,uint32_t Size)
{
	uint32_t EndAddr=StartAddr+Size; 
	uint32_t PageStart = StartAddr/0x800 ; 
	uint32_t PageEnd = (EndAddr-1)/0x800 ; 
	uint32_t EraseCounter = 0x0;
	uint32_t i,j;
	static uint32_t Data32;
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	

	for(EraseCounter=0; EraseCounter< (PageEnd - PageStart+1); EraseCounter++)
	{
		FLASH_ErasePage(FLASH_ADDR_RESERVE); //擦除保留页
		if(EraseCounter == 0)//第一页
		{
			if(PageEnd == PageStart)//只有一页数据
			{
				//写保留页
				for(i=0;i<0x200;i++)
				{
					Flash_Read( StartAddr/0x800*0x800+i*4,(uint8_t*)&Data32, 4);
					FLASH_ProgramWord( FLASH_ADDR_RESERVE+i*4, Data32);
				}		
				FLASH_ErasePage(StartAddr); //擦除当前页
				for(i=0,j=0;i<0x800;i+=4)
				{
					if(i>=(StartAddr%0x800) && i < (StartAddr%0x800)+Size)
					{
						memcpy((uint8_t*)&Data32,Data+j,4);
						j+=4;
					}
					else
						Flash_Read( FLASH_ADDR_RESERVE+i,(uint8_t*)&Data32, 4);
					FLASH_ProgramWord( StartAddr/0x800*0x800+i, Data32);
				}				
			}
			else
			{
				
			}

		
		}
		else if(EraseCounter == PageEnd - PageStart)//最后一页
		{
		}
		else//中间页
		{
			
		}
	}


  FLASH_Lock(); 

}

void Flash_rdp(void)
{
	FLASH_ReadOutProtection(ENABLE);
	
}


void Flash_Read(uint32_t StartAddr,uint8_t * Buffer, uint32_t Size)
{
	uint32_t i=0;
	uint32_t Index=0;
	uint32_t Address = StartAddr;
	uint32_t Data32=0;
	///__disable_irq();
	while (1)
	{
		Data32 = *(__IO uint32_t *)Address;
		for(i=0;i<4;i++)
		{
			if(Index<Size)
				Buffer[Index++]= (uint8_t)(Data32 >> (8*(3-i)));
			else
				return;
		}
		Address +=4;
	}
	///__enable_irq();
}

void Flash_Read_for_download(uint32_t StartAddr,uint8_t * Buffer, uint32_t Size)
{
	uint8_t *p;
	p = (uint8_t*)StartAddr;
	memcpy(Buffer,p,Size);
}

