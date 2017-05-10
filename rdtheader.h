#ifndef _RDT_STRUCT
#define _RDT_STRUCT
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

struct RDTheader{
	uint16_t length;
	uint16_t checksum;
	uint32_t offset;
	int padding :29;
	unsigned int frag :1;
	unsigned int xmit :1;
	unsigned int ack :1;
};

#define RDT_PACKLEN 512
#define RDT_HEADLEN 12

struct RDTpacket{
	struct RDTheader header;
	char body[RDT_PACKLEN - RDT_HEADLEN];
};

uint16_t checksum_of(void * buffer, size_t length){
	uint32_t intermediate;
	uint16_t sum = 0;
	uint16_t term;
	size_t i;
	
	for(i = 0; i < length; i+=2){
		term = 0;
		memcpy((void*)&term,buffer+i,2);
		
		intermediate = (uint32_t)sum + (uint32_t)term;
		sum = (uint16_t) (intermediate / 0x100 + (intermediate % 0x100));
		sum = 0xFFFF - sum;
	}
	return sum;
}

#endif
