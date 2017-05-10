#include "rdtinterface.h"
#include "rdtheader.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>




int main(int argc, char** argv){
	FILE *infile;
	
	if(argc != 4){
		fprintf(stderr,"usage: %s <filename> <IP address> <port number>\n", argv[0]);
		return -1;
	}
	
	infile = fopen(argv[1],"R");
	
	
	return 0
}