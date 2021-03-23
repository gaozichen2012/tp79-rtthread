#ifndef __FRIEND_H_
#define __FRIEND_H_




//每个friend 存32个byte  ,最大存 256个好友   256*32 = 8k
typedef struct
{
	uint8_t friend_id[4];
	uint8_t friend_name[26];
	uint16_t friend_cnt;	
}friend_t,*friend_pt;


void friend_init(void);
void friend_deinit(void);
uint32_t friend_read_cnt(void);
uint8_t friend_add(friend_t fd);
uint8_t friend_del(uint8_t index);
uint8_t friend_read(uint8_t index, friend_t* fd);

#endif







