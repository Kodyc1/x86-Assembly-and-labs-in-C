
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  // struct sockaddr_in{}
#include <netdb.h>
#include <string.h>


// SERVER SIDE

int main(int argc, char** argv){


	if (argc != 2){
		printf("Command format is: \"add_server port_number\"\n");
		exit(1);
	}
	else if (argv[1] == NULL){
		printf("Enter valid port number\n");
		exit(1);
	}


	// sockets and port number and length of client address
	int socketfd, newsocketfd, port_number;

	socklen_t client_length;


	// buffer for holding read/write messages
	char buffer[256];


	// server client addresses
	struct sockaddr_in server_address, client_address;


	// initialize socket and zero out the address
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*) &server_address, sizeof(server_address));


	// get port number
	port_number = atoi(argv[1]);


	// populate server address
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port_number);
	server_address.sin_addr.s_addr = INADDR_ANY;


	// bind socket to server address struct
	bind(socketfd, (struct sockaddr*) &server_address, sizeof(server_address));




	// listen for connection
	listen(socketfd,5);

	printf("Address server started\n");

	while(1){

	// HARDCODED VALUES IN SERVER
	// harris@ics.uci.edu  Ian G. Harris
	// joe@cnn.com         Joe Smith
	// jane@slashdot.org   Jane Smith

	
	// finding a client socket to accept
 	client_length = sizeof(client_address);

	newsocketfd = accept(socketfd, (struct sockaddr*) &client_address,  &client_length);

	if(newsocketfd < 0){printf("accept didn't work\n");}

/*
	bzero(buffer,256);

	int n = read(newsocketfd, buffer, 255);

	if(n < 0){printf("gg\n");}

	printf("message %s\n", buffer);

	n = write(newsocketfd, "got it", 6);
	close(newsocketfd);
	close(socketfd);
*/

	
	while(1){
	
		
		// zero out buffer for reading incoming client requests
		bzero(buffer,256);

		int n = read(newsocketfd, buffer, 255);

		char substring[256];

		strncpy(substring, buffer+1, buffer[0]);

		substring[buffer[0]] = '\0';

		//printf("size: %d substring: %s\n", buffer[0], substring);

		// reply to client depending on request
		char* str;

		if (strcmp(substring, "harris@ics.uci.edu") == 0){
			str = "Ian G. Harris\n";
		}
		else if (strcmp(substring, "joe@cnn.com") == 0){
			str = "Joe Smith\n";
		}
		else if (strcmp(substring, "jane@slashdot.org") == 0){
			str = "Jane Smith\n";
		}
		else if (strcmp(substring, "+++") == 0){
			break;
		}
		else {
			str = "unknown\n";
		}

		n = write(newsocketfd, str, strlen(str));

	}

	// close sockets
	close(newsocketfd);

	}
	close(socketfd);		

	return 0;
}




