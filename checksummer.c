#include "rdtheader.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
	size_t i;
	uint16_t sum;
	struct RDTpacket testpacket;

	for(i = 1; i< argc; ++i){
		bzero((void*)&testpacket,sizeof(testpacket));
		printf("%s\t",argv[1]);
		testpacket.header.length = strlen(argv[i]);
		memcpy((void *)&(testpacket.body),argv[i],testpacket.header.length);
		sum = checksum_of((void*)&testpacket,testpacket.header.length + sizeof(testpacket.header));
		printf("%x\t", sum);
		testpacket.header.checksum = sum;
		sum = checksum_of((void*)&testpacket,testpacket.header.length + sizeof(testpacket.header));
		printf("%x\n", sum);		
	}

	return 0;
}