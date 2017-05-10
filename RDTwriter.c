#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "rdtinterface.h"

char bigbuffer[8*1024*1024];


int main(int argc, char** argv){
	int sockfd = rdt_socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in inspec, outspec;
	struct sockaddr_in checkspec;
	socklen_t outsocklen = sizeof(outspec);
	socklen_t insocklen = sizeof(inspec);
	socklen_t checklen = sizeof(checkspec);
	size_t msglength = 0;
	FILE* outfile = NULL;
	int filenum = 0;
	char filename[128];


	if(sockfd < 0){
		fprintf(stderr,"Error on socket\n");
		return -1;
	}

	bzero(&inspec, sizeof(inspec));
	inspec.sin_family = AF_INET;
	if(argc == 2){
		inspec.sin_port = htons(atoi(argv[1]));
	}else
		inspec.sin_port = htons(0);
	inspec.sin_addr.s_addr = INADDR_ANY;

	if( 0 > bind(sockfd,(struct sockaddr *) &inspec, insocklen)){
		fprintf(stderr,"Error on bind\n");
		return -2;
	}
	fprintf(stderr,"bound\n");

	getsockname(sockfd, (struct sockaddr *)&checkspec, &checklen);
	fprintf(stderr, "Using port %d\n", ntohs(checkspec.sin_port));

	while(1){
		bzero(&outspec, sizeof(outspec));
		msglength = rdt_recv(sockfd, bigbuffer, 0, 8*1024*1024, (struct sockaddr *)&outspec, &outsocklen);
		sprintf(filename,"file%d", filenum++);
		outfile = fopen(filename,"r");
		fprintf(stderr, "file %s\n", filename);
		fwrite(bigbuffer, msglength, 1, outfile);
		fclose(outfile);
	}


	return 0;
}
