#include "stm32f10x.h"
#include "my_string.h"
#include "string.h"
#include <ctype.h>
#include "GT21L16S2Y.H"

void str_to_hex(uint8_t *string,uint8_t *pout,uint16_t strlen)
{  
    uint16_t i;  
    unsigned char highByte, lowByte;  
      
    for (i = 0; i < strlen; i += 2)  
    {  
        highByte = toupper(string[i]);  
        lowByte  = toupper(string[i + 1]);  
  
        if (highByte > 0x39)  
            highByte -= 0x37;  
        else  
            highByte -= 0x30;  
  
        if (lowByte > 0x39)  
            lowByte -= 0x37;  
        else  
            lowByte -= 0x30;  
  
        pout[i / 2] = (highByte << 4) | lowByte;  
    }  
    return ;  
}  


void strHEX_uincode_to_gb2312( uint8_t *user_id_hex_unicode,uint8_t lenth, uint8_t *user_id_hex_gb2312)
{
		uint16_t gb2312_dat16,uin_dat16;
		uint8_t j,k=0;
		//unicode string to GB2312 dat
		for(j=0; j< lenth; j+=2)
		{
			if(user_id_hex_unicode[j+1] != 0x00)
			{
				uin_dat16 = (user_id_hex_unicode[j]) + (user_id_hex_unicode[(j+1)]<<8);
				gb2312_dat16 = unicode_to_gb2312(uin_dat16);
				memcpy(&user_id_hex_gb2312[k],(uint8_t *)&gb2312_dat16,2);
				k+=2;
				
			}
			else
			{
				user_id_hex_gb2312[k++] = (user_id_hex_unicode[j]);
			}
			if(k>=lenth) return;
		}			
}



int get_weekday(int y,int m, int d)
{
  int iWeek;
	if(m==1||m==2) 
	{
		m+=12;
		y--;
	}
	iWeek=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
	return iWeek;
} 

void hex_to_str(uint8_t *pbDest, uint8_t *pbSrc, uint16_t nLen)
{

	uint8_t ddl,ddh;
	uint16_t i;
	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}
	pbDest[nLen*2] = '\0';
	
}

uint8_t  uint16_to_unicode_decstr(uint8_t *str_uni,uint16_t dec )
{
	uint8_t i;
	uint16_t num = 1;
	for(i=0;i<5;i++)
	{
		num *= 10;
		if(dec < num)
			break;
	}
  num = 10;
	memcpy(str_uni,"3000300030003000",(i+1)*4);
	if(i == 0)
		str_uni[1] = dec%10 + '0';
	else if(i==1)
	{
		str_uni[5] = dec%10 + '0';
		str_uni[1] = dec/10 + '0';
	}
	else if(i == 2)
	{
		str_uni[9] = dec%10 + '0';
		str_uni[5] = dec/10 + '0';
		str_uni[1] = dec/100 + '0';			
	}
	else if(i == 3)
	{
		str_uni[13] = dec%10 + '0';
		str_uni[9] = dec/10 + '0';
		str_uni[5] = dec/100 + '0';
		str_uni[1] = dec/1000 + '0';		
	}
	return (i+1)*4;			
}

/*
uint32_t swapInt32(uint32_t value)  
{  
     return ((value & 0x000000FF) << 24) |  
               ((value & 0x0000FF00) << 8) |  
              ((value & 0x00FF0000) >> 8) |  
               ((value & 0xFF000000) >> 24) ;  
}

*/
