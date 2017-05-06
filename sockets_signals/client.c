/*
Mengchen Yang  
my1489

The first program receiver.c  runs in an infinite loop receiving alpha numeric strings as input from the user, one line at a time. After reading one line from the standard input, this program sends this information to the other program only if the line contains the secret code "C00L". The sharing of data between the two processes should take place via shared memory.

The program receiver.c will now be the client program that will interact with the user. Call it client.c.

The client and server programs have the same functionality as described in Assignment 3 except that they should use sockets as a communication mechanism that will allow the client and the server to be run on different machines.
*/

/*
	gcc -o client client.c
*/

/*
	./client 127.0.0.1 3000  		//Pass the server ip address and port number in as two arguments when run the client
*/



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int socketfd, portnum, str_len, i;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    struct hostent *server;

    if (argc < 3) {							//check whether server ip address and port number are provided or not
       fprintf(stderr,"Error,host ipaddress and port needed\n");
       exit(EXIT_FAILURE);
    }
    portnum = atoi(argv[2]);

    socketfd = socket(AF_INET, SOCK_STREAM, 0);				//create a new socket
    if (socketfd < 0){
        perror("Error on opening socket");
		exit(EXIT_FAILURE);
	 }
    server = gethostbyname(argv[1]);
    if (server == NULL){
        fprintf(stderr,"Error,no such host\n");
        exit(EXIT_FAILURE);
    }

    memset((char*)&server_addr,0,sizeof(server_addr));		//initialize the server_addr
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&server_addr.sin_addr.s_addr,
         server->h_length);
    server_addr.sin_port = htons(portnum);

    if (connect(socketfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){		//connect to the server
        perror("Error on connecting");
		exit(EXIT_FAILURE);
	}

	memset(buffer,0,BUFFER_SIZE);							//initialize buffer

    while(1){
		printf("Please enter the message: ");
        fgets(buffer, BUFFER_SIZE, stdin);					//read from the user input
		str_len = strlen(buffer);
		for(i = 0; i< str_len - 1; i++){					//check whether user enter an alpha numeric strings or not
			if((buffer[i] >= 0x30 && buffer[i] <= 0x39) ||(buffer[i] >= 0x41 && buffer[i] <= 0x5A)||(buffer[i] >= 0x61 && buffer[i] <= 0x7A))
				continue;									
			else
				break;
		}

        if(strstr(buffer, "COOL") != NULL){

	    	if (write(socketfd,buffer,strlen(buffer)) < 0) {	//write the user input to socket
       			perror("Error on writing to socket");
				exit(EXIT_FAILURE);
	 		}
			else {
    			if (read(socketfd,buffer,BUFFER_SIZE) < 0) {	//read the reply form the server
        			perror("Error on reading from socket");
					exit(EXIT_FAILURE);
	 	   	 	}
	   	 		printf("%s\n",buffer);
			}
		}
	}
    return EXIT_SUCCESS;
}