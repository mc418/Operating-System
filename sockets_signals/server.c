/*
Mengchen Yang
my1489

The second program processor.c creates an output file secrets.out and waits for user input to be sent by the receiver program. As soon as one line is received from the receiver, it counts the number of digits in that line and dumps the digit count along with the original line in the secrets.out file. This program also runs in an infinite loop.

The processor.c will be the backend program – call it server.c. 

The client and server programs have the same functionality as described in Assignment 3 except that they should use sockets as a communication mechanism that will allow the client and the server to be run on different machines.

When the user presses Ctrl-C on the server.c terminal, the server program should display a summary, which should show the number of lines received so far and the total digit count across all input seen by the server.
*/

/*
    gcc -o server server.c
*/

/*
    ./server 3000              // Pass the port number (2000 to 65535) in as an argument when run the server 
*/



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 1024


int line_num, sum_digitcounter;

/* Signal Handler for SIGINT */
void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf(" the number of lines received so far is %d and the total digit count is %d \n", line_num, sum_digitcounter);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
     int socketfd, acceptfd, portnum;
	 socklen_t clilen;
     char buffer[BUFFER_SIZE];
     struct sockaddr_in server_addr, client_addr;
     int i, digit_counter;
	 FILE *fpt; 
	 signal(SIGINT, sigintHandler);

	 memset(buffer,0,BUFFER_SIZE);                          //initialize the buffer

	 line_num = 0;
	 sum_digitcounter = 0;

     if (argc < 2) {                                        //check whether port number is provided or not
         fprintf(stderr,"Error, Port not provided\n");
         exit(EXIT_FAILURE);
     }

     socketfd = socket(AF_INET, SOCK_STREAM, 0);           //create a new socket

     if (socketfd < 0){
        perror("Error on opening socket");
		exit(EXIT_FAILURE);
	 }

	 memset((char*)&server_addr,0,sizeof(server_addr));    //initialize the server_addr
     portnum = atoi(argv[1]);
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = INADDR_ANY;
     server_addr.sin_port = htons(portnum);

	 /* binds a socket to an address */
     if (bind(socketfd, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0) {
        perror("Error on binding");
 		exit(EXIT_FAILURE);
     }

     listen(socketfd,5);                                    //listen on the socket for connections
     clilen = sizeof(client_addr);

	 /* The accept() system call causes the process to block until a client connects to the server. */
     acceptfd = accept(socketfd, (struct sockaddr *) &client_addr, &clilen);
     if (acceptfd < 0){
        perror("Error on accept");
 		exit(EXIT_FAILURE);
     }

	 while(1){
     	if (read(acceptfd,buffer,BUFFER_SIZE) < 0) {        //read from socket
            perror("Error on reading from socket");
 		    exit(EXIT_FAILURE);
    	}

		line_num = line_num + 1;                            //count line number
		digit_counter  = 0;                                 //count digit number in on line
		for( i = 0; i< strlen(buffer); i++){
            if(buffer[i] >= 0x30 && buffer[i] <= 0x39){
                digit_counter++;
            }
        }    
        sum_digitcounter += digit_counter;                   //count sum number


		/*  dumps the digit count along with the original line in the secrets.out file */
		fpt = fopen("secrets.out","a");
		fprintf(fpt,"the user input: %sthe number of digits: %d \n",buffer, digit_counter); 
		fclose(fpt); 

		memset(buffer,0,strlen(buffer));                    //clear the buffer

	    if (write(acceptfd,"Message received",18) < 0) {    //inform the client
        perror("Error on writing to socket");
 		exit(EXIT_FAILURE);
		}
    }
    return EXIT_SUCCESS;
}