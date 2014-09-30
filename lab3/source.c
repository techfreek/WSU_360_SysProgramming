#include "source.h"

int executeCMD(char* args[], int numArgs) {
	//Look for I/O redirection calls
		//If < then takes inputs from an infile
			//Close pd[0]
			//open infile for reading
		//If > sends outputs to an outfile
			//close pd[1]
			//open outfile for writing
		//Else If >> appends outputs to outfile
			//close pd[1]
			//open outfile for appending 'a'
	int i = 0,
		firstIO = -1,
		r,
		pid;

	int pd[2];
	r = pipe(pd);
	FILE *infile, //verify that these do infact go into the right file descriptors
		 *outfile;

	char* command = (char*)malloc(sizeof(char) * 128);

	for(; i < numArgs; i++) {
		if(args[i] == "<") {
			if(firstIO < 0) {
				firstIO = i;
			}
			close(pd[0]);
			close(1);
			//open infile for read
			infile = fopen(args[i + 1], "r");
		} else if(args[i] == ">") {
			if(firstIO < 0) {
				firstIO = i;
			}
			close(pd[1]);
			close(0);
			//open outfile for writing
			outfile = fopen(args[i + 1], "w");
		} else if(strcmp(args[i], ">>") == 0) {
			if(firstIO < 0) {
				firstIO = i;
			}
			close(pd[1]);
			close(0);
			//open outfile for appending
			outfile = fopen(args[i + 1], "a");
		}
	}


	//Fork process
	pid = fork();
	if(!pid) { //Parent
		//Wait for child to terminate
		pid = wait(&r);

		//print exit status code
		printf("Child died with status: %d", r);

		//return to main loop
	} else { //child
		//execute command (will be in '/bin/')
		strcpy(command, "/bin/");
		strcat(command, args[0]);
		execve(command, args, enviro);
	}
}

int executeCMDs(char* cmds[], int numCommands) {
	//For each command
		//create array of char pointers to the arguments, pass to 
		//call execute CMD
	char* args[64];
	char* token = (char*)malloc(sizeof(char) * 128);

	int i = 0,
		j = 0;
	for(; i < numCommands; i++) {
		token = strtok(cmds[i], " ");
		j = 0;
		while(token != NULL) {
			args[j] = token;
			j++;
			token = (char*)malloc(sizeof(char) * 128);
			token = strtok(NULL, " ");
		}
		args[j] = NULL;
		executeCMD(args, j);
	}
}