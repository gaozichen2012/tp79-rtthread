#ifndef _MACHINE_CONFIG_
#define _MACHINE_CONFIG_

#define THIS_VERSION   TYT_VERSION

#define TYT_VERSION				1 //����ͨ�汾
#define ENG_VERSION				2 //Ӣ�İ�

//79boot����İ汾����һ����Ч�������������Զ���app�ζ�ȡ����İ汾
#define VER_SOFT		"TYT_TP79V02_V190" 


#if (THIS_VERSION == TYT_VERSION)

#elif THIS_VERSION == ENG_VERSION
#define LANGUAGE_ENGLISH
#endif

#endif

