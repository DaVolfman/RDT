#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "rdtinterface.h"
#include <sys/stat.h>
#include <netdb.h>

char bigbuffer[8*1024*1024];

int main(int argc, char** argv){
	FILE* infile;
	struct sockaddr_in outspec;
	struct hostent* server;
	socklen_t outaddrlen = sizeof(outspec);
	int sockfd = rdt_socket(AF_INET, SOCK_DGRAM, 0);
	int portnum;
	struct stat filestat;

	if(argc != 4){
		fprintf(stderr, "%s <filename> <address> <port number>\n", argv[0]);
		return -1;
	}

	stat(argv[1], &filestat);
	infile = fopen(argv[1], "r");
	server = gethostbyname(argv[2]);
	portnum = atoi(argv[3]);

	bzero(&outspec, sizeof(outspec));
	outspec.sin_port = htons(portnum);
	outspec.sin_family = AF_INET;
    bcopy(server->h_addr_list[0],
          (char *)&outspec.sin_addr.s_addr,
           server->h_length);
    //outspec.sin_addr.s_addr = htonl(outspec.sin_addr.s_addr);

	fread(bigbuffer,filestat.st_size, 1, infile);
	rdt_send(sockfd, bigbuffer, filestat.st_size, 0, &outspec, outaddrlen);
	rdt_close(sockfd);

	return 0;
}
