//#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
	int portnum;
	int sockfd;
	struct sockaddr_in sockinfo;
	struct sockaddr_in sourceinfo;
	socklen_t sourcesize = sizeof(sourceinfo);
	char buffer[1501];
	
	if(argc == 2){
		portnum = atoi(argv[1]);
	}else{
		return -1;
	}
	bzero(&sockinfo, sizeof(sockinfo));
	
	sockinfo.sin_port = htons(portnum);
	sockinfo.sin_family = AF_INET;
	sockinfo.sin_addr.s_addr = inet_addr("0.0.0.0");
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	bind(sockfd, (struct sockaddr *) &sockinfo, sizeof(sockinfo));
	
	while( 1 ){
		bzero(buffer, 1501);
		bzero(&sourceinfo, sizeof(sourceinfo));
		recvfrom(sockfd, &buffer, 1500, 0, (struct sockaddr *)&sourceinfo, &sourcesize);
		fprintf(stderr, "recieved message from %s:%d\n", inet_ntoa(sourceinfo.sin_addr), ntohs(sourceinfo.sin_port));
		sendto(sockfd, &buffer, 1500, 0, (struct sockaddr *)&sourceinfo, sourcesize);		
	}
	
	return 0;
}