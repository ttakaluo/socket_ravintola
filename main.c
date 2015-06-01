/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>					//atoi, exit
#include <string.h> 					//memset
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>  					//fork
#include "print_error.h" 				//error-handling function => void error(char *msg)
#include "talk_to_client.h"  				//child-process read-socket

int main(int argc, char *argv[]){

	if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
	}

	int sockfd;						//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // ipv4 connection, sequenced two-way
	if (sockfd < 0) 
		error("ERROR opening socket");

	struct sockaddr_in serv_addr;			 
	int portno;

	memset((char *) &serv_addr ,0,sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;   //accept all interfaces
	serv_addr.sin_port = htons(portno);		//convert network byte order to host byte order

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	struct sockaddr_in cli_addr;
	socklen_t clilen;

	while(1){						//Loop to listen and spawn fork
	
     		listen(sockfd,5);
		clilen = sizeof(cli_addr);
		int newsockfd;
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");

		pid_t childPid;
		childPid = fork();
		
		if (childPid == 0) {
			talk_to_client(newsockfd);
			return 0;
		}
		else if (childPid == -1) {
			error("Problem while forking");
			return 1;
		}
	} 
	return 0; 					//should never get here
}
