#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include "rdtheader.h"
#include "rdtinterface.h"
#include <stdio.h>

const int RDT_RETRY_LIMIT = 3;

static int drop_q = 0;
static int corrupt_q = 0;

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

int rdt_socket(int addr_family, int type, int protocol){
	return socket(addr_family, type, protocol);
}
int rdt_bind(int sockd, const struct sockaddr* localaddr, socklen_t addrlen){
	return bind(sockd, localaddr, addrlen);
}
int rdt_close(int fd){
	return close(fd);
}

int rdt_send(int sockd, char* buffer, int bufflen, int flags, struct sockaddr* addrto, socklen_t addrlen){
	struct sockaddr_in response_addr;
	socklen_t respaddr_len;
	struct RDTpacket senddata;
	struct RDTpacket response;
	char* readpoint = buffer;
	char* endpoint = buffer + bufflen;
	int bytessent = 0;
	int retries = 0;
	int bytesleft = bufflen;
	int selectcode;
	int maxpayload = RDT_PACKLEN - sizeof(senddata.header);
	fd_set sel_set;
	struct timeval timeout;
	int probC, probD;
	
	srand(time(NULL));
	
	senddata.header.xmit = 1;
	senddata.header.ack = 0;
	
	while(readpoint < endpoint){
		if(retries > RDT_RETRY_LIMIT)
			return -bytessent;
		
		probC = rand() % 1000;
		probD = rand() % 1000;
		
		senddata.header.checksum = 0;
		senddata.header.offset = readpoint - buffer;
		bytesleft = endpoint - readpoint;
		if(bytesleft <= maxpayload){
			senddata.header.frag = 0;
			senddata.header.length = bytesleft;
		}else{
			senddata.header.frag = 1;
			senddata.header.length= maxpayload;
		}
		memcpy((void *)&(senddata.body),(void*)readpoint,senddata.header.length);
		senddata.header.checksum = checksum_of((void*)&senddata,senddata.header.length + sizeof(senddata.header));
		if(probC < corrupt_q)
			senddata.body[1] = senddata.body[1] ^ 0xFF;
		if(probD >= drop_q)
			sendto(sockd, (void*)&senddata, senddata.header.length + sizeof(senddata.header), flags, addrto, addrlen);
		else
			fprintf(stderr,"Packet intentionally dropped.\n");
		
		FD_ZERO(&sel_set);
		FD_SET(sockd, &sel_set);
		timeout.tv_usec = 0;
		timeout.tv_sec = 1;
		selectcode = select(FD_SETSIZE,&sel_set,NULL,NULL,&timeout);
		if(selectcode == 1){
			recvfrom(sockd, (void*)&response, RDT_PACKLEN, flags, (struct sockaddr *)&response_addr, &respaddr_len);
			if(response.header.ack && response.header.offset == (senddata.header.offset + senddata.header.length)){
				readpoint = readpoint + senddata.header.length;
				bytessent = bytessent + senddata.header.length;
				retries = 0;
			} else {
				fprintf(stderr,"Timeout: Missing Ack.\n");
				retries++;
			}
		} else {
			retries++;
		}
		
	}
	
	return bytessent;
}

int rdt_recv(int sockd, char* buffer, int bufflen, int flags, struct sockaddr* addrfrom, socklen_t* addrlen){
	struct RDTpacket thepacket;
	size_t received_to = 0;
	uint16_t sumrcvd, sumcalcd;
	int done = 0;
	
	while(!done && received_to < bufflen){
		if((recvfrom(sockd, (void*)&thepacket, sizeof(thepacket), flags, addrfrom, addrlen) <= 0))
			return 0 - received_to;
			
		sumrcvd = thepacket.header.checksum;
		thepacket.header.checksum = 0;
		sumcalcd = checksum_of((void*)&thepacket,sizeof(thepacket.header)+thepacket.header.length);
		
		thepacket.header.xmit = 0;
		thepacket.header.ack = 1;
		if(sumcalcd != sumrcvd || thepacket.header.offset != received_to){
			thepacket.header.offset=received_to;
			fprintf(stderr, "Incorrect or corrupt packet received\n");
		}else{
			if((received_to + thepacket.header.length) > bufflen)
				return 0 - received_to;
			memcpy(buffer+received_to,(void *)(thepacket.body),thepacket.header.length);
			received_to += thepacket.header.length;
			if(thepacket.header.frag == 0)
				done = 1;
			thepacket.header.offset=received_to;
		}
		thepacket.header.length=0;
		thepacket.header.checksum=0;
		thepacket.header.checksum = checksum_of((void*)&thepacket,sizeof(thepacket.header));
		sendto(sockd,(void*)&thepacket,sizeof(thepacket),flags,addrfrom,*addrlen);
	}
	
	return received_to;
}

int set_drop_q(int mils){
	if(mils < 0 || mils > 1000)
		return 0;
	drop_q = mils;
}

int set_corrupt_q(int mils){
	if(mils < 0 || mils > 1000)
		return 0;
	corrupt_q = mils;
}
