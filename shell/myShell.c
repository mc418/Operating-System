#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

void partCmd(char *cmd, char *argv[])   // use this funciton to seperate command line into array
{		
	int i = 0;
	while (i < 10){						//save seperated cmd in argv[] for further use
		argv[i] = strsep(&cmd, " ");
		i++;
		if (argv[i] == NULL) {
			break;
		}
	}
}						   

int exeCmd(char *argv[])				// execute command
{
	char *error = strerror(errno);
	pid_t pid = fork();					// fork process
	
	if (pid == 0)						// child process 
	{
		 execvp(argv[0], argv);
		 printf("myShell: %s: %s\n", argv[0], error); 
		 return 0;						//error occurs, because child process could not do execvp
	}
	else{								// parent process
		int child;
		waitpid(pid, &child, 0);		// wait until child process finishes
		return 1;
	}
}

int main()
{
	char cmd[100];
	char *argv[10];

	while(1) {
		char *user = getenv("USER");	// information of user
		printf("%s@myShell->", user);

		if (fgets(cmd, sizeof(cmd), stdin) == NULL) // fgets returns NULL on Ctrl+D
		{											 // so exit the loop on NULL
			break;
		}
		if(cmd[strlen(cmd)-1] == '\n') {// delete '\n' of cmd in array
			cmd[strlen(cmd)-1] = '\0';
		}

		partCmd(cmd, argv);				// seperate cmd

		if (strcmp(argv[0], "exit") == 0)// check cmd, if cmd is exit, finish the program
		{
			printf("-> exit myShell.\n");
			break;
		}
		else if (exeCmd(argv) == 0)		// execute cmd
		{
			break;
		}	
	}
	return 0;
}