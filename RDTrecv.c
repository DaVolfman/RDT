#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rdtinterface.h"

static const size_t buffsize = 8388608;//8 MB

int main(int argc, char** argv){
	struct sockaddr_in source;
	socklen_t sourcelen = sizeof(source);
	struct sockaddr_in here;
	char buffer[buffsize];
	int status;
	int sockd;
	
	if(argc != 2){
		fprintf(stderr,"Usage: %s <port number>\n",argv[0]);
		return 1;
	}
	
	sockd = socket(AF_INET,SOCK_DGRAM,0);
	
	here.sin_port = htons(atoi(argv[1]));
	here.sin_addr.s_addr = inet_addr("0.0.0.0");
	here.sin_family = AF_INET;
	
	if(bind(sockd,(struct sockaddr *)&here, (socklen_t)sizeof(here)) < 0){
		fprintf(stderr,"Error binding socket to port %d\n",atoi(argv[1]));
		return 2;
	}
	
	while(1){
		bzero((void*)&buffer,buffsize);
		status = rdt_recv(sockd,buffer,buffsize,0,(struct sockaddr*)&source,&sourcelen);
		if( status <= 0){
			fprintf(stderr,"Error receiving message.\n");
		}else{
			fprintf(stderr,"New message:\n%s\n",buffer);
		}
	}
	
	return 0;
}