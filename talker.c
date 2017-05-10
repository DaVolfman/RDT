#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
	int timeout;
	int sockfd;
	int status;
	struct sockaddr_in destinfo/*,sourceinfo*/;
	struct sockaddr_in backinfo;
	socklen_t backsize = sizeof(backinfo);
	char message[1501];
	fd_set select_set;
	struct timeval timespec;
	
	if(argc != 4){
		fprintf(stderr, "Usage: %s <port number> <ip address> <timeout ms>\n", argv[0]);
		return 1;
	}else{
		bzero(&destinfo, sizeof(destinfo));
		destinfo.sin_family = AF_INET;
		destinfo.sin_port = htons(atoi(argv[1]));
		destinfo.sin_addr.s_addr = inet_addr(argv[2]);
		timeout = atoi(argv[3]);		
		sockfd = socket(AF_INET,SOCK_DGRAM,0);
		/*bzero(&sourceinfo, sizeof(sourceinfo));
		sourceinfo.sin_family = AF_INET;
		sourceinfo.sin_port = htons(9858);
		sourceinfo.sin_addr.s_addr = inet_addr("0.0.0.0");
		bind(sockfd, (struct sockaddr *)&sourceinfo, sizeof(sourceinfo));*/
	}
	
	while(1){
		printf("Enter message:");
		fgets(message, 1500, stdin);
		sendto(sockfd, message, 1500, 0, (struct sockaddr*)&destinfo, sizeof(destinfo));

		timespec.tv_sec = timeout / 1000;
		timespec.tv_usec = (timeout % 1000) * 1000;
		FD_ZERO( &select_set);
		FD_SET( sockfd, &select_set);
		
		if((status = select(FD_SETSIZE,&select_set,NULL,NULL,&timespec)) == 1){
			recvfrom(sockfd,message, 1500,0, (struct sockaddr*)&backinfo, &backsize);
			printf("Recieved: %s", message);
		}else{
			if(status == 0){
				printf("Timed out after %dms\n", timeout);
			}else if(status == -1){
				printf("Error\n");
			}else{
				printf("Unknown select return %d", status);
			}
			//break;
		}
		
	}
	
	
	return 0;
}