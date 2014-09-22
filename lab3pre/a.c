#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pid;

char line[128];
char token[32][64];
char cwd[128], command[128];

main(int argc, char *argv[ ], char* env[])
{
	char* line;
	char* token;
	char* command;
	char* myargv[32];
	int size;
	int i = 0;
	printf(": ");
	getline(&line, &size, stdin);

	command = (char*)malloc(sizeof(char) * 256);
	token = (char*)malloc(sizeof(char) * 256);
	token = strtok(line, " ");
	while(token != NULL) {
		myargv[i] = token;
		printf("myargv[%d]=%s\n", i, myargv[i]);
		i++;
		token = (char*)malloc(sizeof(char) * 256);
		token = strtok(NULL, " ");
	}


	/*2. Write code to           |-------- n=5--------| 
	input a line, e.g.    b.out this is a test
	tokenize line into inidividual strings as 
	token[0], token[1],....,token[n-1]*/

	//3. // Write code to let myargv[i] point to token[i], (0 <= i < n)
	//myargv[n] = 0;
	pid = getpid();
	printf("proc %d in a.out exec to b.out\n", pid);

	getcwd(cwd,128);     // cwd contains the pathname of CWD  
	printf("cwd = %s\n", cwd);

	strcpy(command, cwd);
	strcat(command, "/bout");
	printf("command: %s\n", command);

	// WRITE CODE to let command = CWD/b.out

	execve(command, &myargv, &env);

	printf("execve failed\n");
}