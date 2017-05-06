/*
Mengchen Yang  
my1489

The first program receiver.c  runs in an infinite loop receiving alpha numeric strings as input from the user, one line at a time. After reading one line from the standard input, this program sends this information to the other program only if the line contains the secret code "C00L". The sharing of data between the two processes should take place via shared memory. 
*/

/*
	gcc -o receiver receiver.c
*/

/*
	./receiver
*/


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXSIZE     27
 
void die(char *s)												// If there's error, exit
{
	perror(s);
	exit(1);
}
 
int main()
{
	int shmid;
	key_t key;
	char *shm, *s, c;
	int infinite_loop = 1;
	char * user_input = malloc(sizeof(char) * BUFSIZ);
	
	key = getuid();												//returns the real user ID of the calling process.
 
	
	if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)	// Create the segment, error occurs if not
		die("shmget");
	
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)			// Attach the segment to data space, error occurs if not
		die("shmat");
 

	
	while (infinite_loop) {										// Put string with "COOL" into memory for processor to read
		
		printf("Enter an alpha numeric string: ");				// ask user to type an alfpa numeric string as input
		fgets(user_input, BUFSIZ, stdin);
		

		if (strstr(user_input, "COOL")) {						// Search "COOL" in the input string and copy to memory
			memcpy(shm, user_input, strlen(user_input));
		
			s = shm;											// "$" is added to the end of the input string
			s += strlen(user_input);
			*s = '$';
		
			while (*shm != '@') {								//  finish reading (from processor.c )
				sleep(1);
			}
		}
	}
 
	if (shmdt(shm) == -1) {
		die("shmdt failed: segment wasn't detached from data space");
	}
	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		die("shmctl failed: ipc didn't close");
	}
}