
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// CLIENT SIDE

int main(int argc, char** argv){
	

	// error handling
	if (argc != 3){
		printf("Error: need 3 arguments (add_client, hostname, portnumber)\n");
		exit(1);
	}


	// socket and port
	int socketfd, port_number;


	// socket server address and server name for client to find
	struct sockaddr_in server_address;
	struct hostent* server;


	// buffer for sending and receiving messages
	char buffer[256];


	// hostent name
	server = gethostbyname(argv[1]);
	if(server == NULL){printf("server is a null\n");exit(0);}

	// port number
	port_number = atoi(argv[2]);

	
	// initialize client socket and zero address
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*) &server_address, sizeof(server_address));



	// populate client's address using server name
	server_address.sin_family = AF_INET;
	bcopy((char*) server -> h_addr, 
		(char*) &server_address.sin_addr.s_addr, 
		server->h_length);
	server_address.sin_port = htons(port_number);


	// connect to socket
	connect(socketfd, (struct sockaddr*) &server_address, sizeof(server_address));
	//printf("connected\n");
	


/*
	printf("please enter message: ");	
	bzero(buffer, 256);
	fgets(buffer,255,stdin);
	printf("why");

	int n;
	printf("%s", "african american");

	if ( write(socketfd, buffer, strlen(buffer))<0){printf("rip\n");}

	printf("position 1\n");
	
	bzero(buffer,256);
	read(socketfd, buffer, 255);
	
	close(socketfd);

*/


	// prompt user with a shell on client side
	char* line = NULL;
	size_t length = 0;
	ssize_t reads;

	while(1){

		printf("> ");

		fflush(stdout);


		// get the line from shell
		reads = getline(&line, &length, stdin);		

		if (reads < 0)
			break;
		if (line[reads-1] == '\n')
			line[reads-1] = '\0';



		// quit condition



		int n; // read/write returns an int, -1 if error

		// zero out buffer and write it to the socket to send to server
		bzero(buffer, 256);
		
		uint8_t length = strlen(line);

		buffer[0] = length;
	
		strcat(buffer, line);

		//printf("%s\n", buffer);

		n = write(socketfd, buffer, strlen(buffer));
		
		if (strcmp(line, "+++") == 0){
			break;
		}


		// zero out buffer to receive message from server and print it
		bzero(buffer, 256);
	
		n = read(socketfd, buffer, 255);
			if(n < 0){printf("error\n");}		

		printf("%s", buffer);

	}

	close(socketfd);

	return 0;

}



