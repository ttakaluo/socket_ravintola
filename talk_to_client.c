#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "print_error.h"
#include "fifod.h"

int talk_to_client(int newsockfd, int logfile){

	//this is the child-process handling incoming communication
	char buffer[256];
	int n;
	memset(buffer,0,256);

	do{
		n = read(newsockfd,buffer,255);
		if (n < 0) {
			error("ERROR reading from socket");
			return 1;
		}
	}
	while (n != 0) ;
	close(newsockfd);
	write(logfile, buffer, strlen(buffer));
	return 0;
}
