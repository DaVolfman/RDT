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

#endif
