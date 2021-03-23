#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#include "stdint.h"
//------------------------------------------------
#ifndef BYTE
typedef unsigned char 			BYTE;
#endif
//------------------------------------------------
#ifndef u8
typedef unsigned char 			u8;
#endif

#ifndef u16
typedef unsigned short 			u16;
#endif

#ifndef u32
typedef unsigned int 			u32;
#endif

#ifndef u64
typedef unsigned long      			u64;
#endif

//------------------------------------------------
#ifndef uint8
typedef unsigned char 			uint8;

#endif

#ifndef uint16
typedef unsigned short 			uint16;
#endif

#ifndef uint32
typedef unsigned int 			uint32;
#endif

#ifndef uint64
typedef unsigned long 			uint64;
#endif
//------------------------------------------------
 #ifndef uint8_t
 #define uint8_t 				unsigned char 
 #endif

 #ifndef uint16_t
 #define uint16_t				unsigned short 
 #endif

 #ifndef uint32_t
 #define uint32_t 			unsigned int 
 #endif

 #ifndef uint64_t
 #define uint64_t 			unsigned long
 #endif

//------------------------------------------------
#ifndef int8
typedef unsigned char 			int8;
#endif

#ifndef int16
typedef signed short 			int16;
#endif

#ifndef int32
typedef signed int 				int32;
#endif

#ifndef int64
typedef signed long 				int64;
#endif

#ifndef bool
typedef enum
{
	FALSE=0,
	TRUE,
}bool;
#endif

#ifndef BOOL
#define BOOL bool
#endif

#ifndef BOOL_t
#define BOOL_t bool
#endif

//开关状态
typedef enum
{
	OFF=0,
	ON=1,
}OnOff_t;

typedef enum
{
	FAILED,
	SUCCESSED,
}OpResault_t;

typedef enum
{
	DIR_DOWN=0,	//下
	DIR_UP,			//上
	DIR_LEFT, 	//左
	DIR_RIGHT,	//右
	DIR_FRONT,	//前
	DIR_BACK,		//后
}Dirction_t;



#define GPIO_PORT    (GPIO_TypeDef*)
#define GPIO_PIN     uint16_t

#endif
