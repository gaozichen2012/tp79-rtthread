#ifndef __MY_STRING_H_
#define __MY_STRING_H_


#define ENABLE 1
#define DISENABLE 0

void str_to_hex(uint8_t *string,uint8_t *pout,uint16_t strlen);

void strHEX_uincode_to_gb2312( uint8_t *user_id_hex_unicode,uint8_t lenth, uint8_t *user_id_hex_gb2312);//unicode转 gb2312
void hex_to_str(uint8_t *pbDest, uint8_t *pbSrc, uint16_t nLen);
int get_weekday(int y,int m, int d);
uint8_t  uint16_to_unicode_decstr(uint8_t *str_uni,uint16_t dec );//uint16 转10进制unicode 字符串
//uint32_t swapInt32(uint32_t value) ;

#endif


