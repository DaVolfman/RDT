#include "rdtinterface.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>



int main(int argc, char** argv){
	struct stat filestat;
	void* buffer = NULL;
	int sockd, filed;
	int status;
	struct sockaddr_in dest, dummy;
	socklen_t dummysize;
	
	if(argc != 6){
		fprintf(stderr,"usage: %s <filename> <IP address> <port number> <drop rate mils> <error rate mils>\n", argv[0]);
		return -1;
	}
	
	stat(argv[1],&filestat);
	filed = open(argv[1],O_RDONLY);
	buffer = mmap(NULL,filestat.st_size,PROT_READ,MAP_PRIVATE,filed,0);
	
	sockd = rdt_socket(AF_INET,SOCK_DGRAM,0);
	dest.sin_port = htons(atoi(argv[3]));
	dest.sin_addr.s_addr = inet_addr(argv[2]);
	dest.sin_family = AF_INET;
	
	set_drop_q(atoi(argv[4]));
	set_corrupt_q(atoi(argv[5]));
	
	status = rdt_send(sockd,(char*)buffer,filestat.st_size,0,(struct sockaddr*)&dest,(socklen_t)(sizeof(dest)));
	
	if(status <= 0){
		fprintf(stderr,"Error sending message\n");
		return -1;
	}
		
	
	close(sockd);
	close(filed);
	
	return 0;
}