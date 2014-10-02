#include "source.h"

extern char* enviro[];
extern char* home;

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

	//Fork process
	pid = fork();
	if(pid) { //Parent
		//Wait for child to terminate
		pid = wait(&r);

		//print exit status code
		printf("Child died with status: %d\n", r);
		printf("Error: %s\n", strerror(errno));
		//return to main loop
	} else { //child
		child:
		int pd[2];
		
		//r = pipe(pd);
		FILE *infile, //verify that these do infact go into the right file descriptors
			 *outfile;
		
		char* command = (char*)malloc(sizeof(char) * 128);
		printf("Args: [");

		for(; i < numArgs; i++) {
			printf("%s, ", args[i]);
			if(strcmp(args[i], "<") == 0) { //input
				if(firstIO < 0) {
					firstIO = i;
				}
				//close(pd[0]);
				close(0);
				//open infile for readfil
				infile = open(args[i + 1], O_RDONLY); //open(argv[1],O_RDONLY
				dup(infile);
			} else if(strcmp(args[i], ">") == 0) { //overwriting
				if(firstIO < 0) {
					firstIO = i;
				}
				//close(pd[1]);
				close(1);
				//open outfile for writing
				outfile = open(args[i + 1], O_CREAT);
				dup(outfile);
			} else if(strcmp(args[i], ">>") == 0) { //appending
				if(firstIO < 0) {
					firstIO = i;
				}
				//close(pd[1]);
				close(1);
				//open outfile for appending
				outfile = open(args[i + 1], O_APPEND);
				dup(outfile);
			}
		}
		printf("]\n");

		if(firstIO > 0) {
			//If there is any I/O redirection, adjust the array so the program doesn't get confused by those inputs
			args[firstIO] = NULL;
			numArgs = firstIO;
		}


		//execute command (will be in '/bin/')
		printf("This is child\n");
		strcpy(command, "/bin/");
		strcat(command, args[0]);
		printf("command: %s\n", command);
		execv(command, args, enviro);
		if(infile) {
			close(infile);
		}
		if(outfile) {
			close(outfile);
		}
		printf("Error: %s\n", strerror(errno));
	}
}

int executeCMDs(char* cmds[], int numCommands) {
	//For each command
		//create array of char pointers to the arguments, pass to 
		//call execute CMD

	char* args[64] = {0}; /* THIS IS NOT RUN, NO CLUE WHY */
	char* token = (char*)malloc(sizeof(char) * 128);

	int i = 0,
		j = 0;
	for(; i < numCommands; i++) {
		token = strtok(cmds[i], " ");
		//args[0] = (char*)malloc(sizeof(char) * 128);
		//strcpy(args[0], token);
		//args[0] = token;
		j = 0; //the first command is the filename
		while(token != NULL) {
			args[j] = strdup(token);
			//args[j] = token;
			j++;
			
			token = strtok(NULL, " ");
		}
		args[j] = NULL;
		executeCMD(args, j);
		memset(args, 0, 64);
	}
}