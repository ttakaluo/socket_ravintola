/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h> //exit
#include <string.h> //memset
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>  //fork

void error(char *msg)
{
    perror(msg);
    exit(1);
}

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

int main(int argc, char *argv[]){

	//int sockfd, newsockfd, portno, clilen;
	//char buffer[256];
	//struct sockaddr_in serv_addr, cli_addr;
	//int n;
	
	// check for arguments
	if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
	}

	// create socket
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);  // ipv4 connection, sequenced two-way
	if (sockfd < 0) 
		error("ERROR opening socket");

	// create bind
	struct sockaddr_in serv_addr;
	int portno;

	memset((char *) &serv_addr ,0,sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;    //accept all interfaces
	serv_addr.sin_port = htons(portno);		 //convert network byte order to host byte order

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	// wait for connect
	struct sockaddr_in cli_addr;
	int clilen;

	while(1){
	
     	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	int newsockfd;
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");

	// when connected, fork

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
	// past to child return from accept
	// loop to wait for new connections

	return 0; 
}
