#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "print_error.h"

int talk_to_client(int newsockfd){

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
	printf("Got this message: %s\n", buffer);
	return 0;
}
