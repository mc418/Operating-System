/*
Mengchen Yang
my1489

The second program processor.c creates an output file secrets.out and waits for user input to be sent by the receiver program. As soon as one line is received from the receiver, it counts the number of digits in that line and dumps the digit count along with the original line in the secrets.out file. This program also runs in an infinite loop.
*/

/*
	gcc -o processor processor.c
*/

/*
	./processor
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 
#define MAXSIZE     27


void die(char *s)										// If there's error, exit
{
	perror(s);
	exit(1);
}
 
int main()
{
	char c;
	int shmid;
	key_t key;
	char *shm, *s;
 
	int infinite_loop = 1;
	int cnt;
	int digit;
	char line[BUFSIZ];
	FILE * secrets;
	int num[10]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	secrets = fopen("secrets.out", "w");				//Create output file 'secrets.out'
	
	key = getuid();										//returns the real user ID of the calling process.
 

	if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)		//Locate the segment, error occurs if not located 
		die("shmget");

	if ((shm = shmat(shmid, NULL, 0)) == (char *) - 1) 	//Attach the segment to data space, error occurs if not attached
		die("shmat");
 
	
	while(infinite_loop){								// Read what the receiver put in the memory and count digits
		
		if (*shm != (int)NULL) {						// program waits for user input
			if (*shm != '@') {							// *shm begins with an '@' while there is no new input. 
														// Thus, the program has to wait for a new input
				
				cnt = 0;
				digit = 0;								// set counter to 0
				memset(line, 0, sizeof(line));			// set line of input to empty
				
				for (s = shm; *s != '$'; s++) {			// count the number of digits in the input string
					if (*s >= '0' && *s <= '9') {
						digit++;
					}
					line[cnt] = *s;						// the string received is passed into a line characters array
					cnt++;
				}
				

				*shm = '@';								// Mark finished reading by inserting an '@' to the memory address

				secrets = fopen("secrets.out", "a");	// the "secrets.out" file is opened and can be appended
				
				if (secrets != NULL) {					// write input string and count digits in "secrets.out"
					fprintf(secrets, "%d: ", digit);
					fputs(line, secrets);
					fclose(secrets);
				}
			}
		}
	}
	

	if (shmdt(shm) == -1) {
		die("shmdt failed: segment wasn't detached from data space");
	}
}