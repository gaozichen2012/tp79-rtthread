#ifndef _MACHINE_CONFIG_
#define _MACHINE_CONFIG_

#define THIS_VERSION   TYT_VERSION

#define TYT_VERSION				1 //特易通版本
#define ENG_VERSION				2 //英文版

//79boot程序的版本仅第一次有效，后续升级会自动从app段读取程序的版本
#define VER_SOFT		"TYT_TP79V02_V190" 


#if (THIS_VERSION == TYT_VERSION)

#elif THIS_VERSION == ENG_VERSION
#define LANGUAGE_ENGLISH
#endif

#endif

