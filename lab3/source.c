#include "source.h"

extern char* enviro[];
extern char* home;

int executeCMD(char* args[], char* args2[], int numArgs, int numArgs2) {
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
		pid,
		piped = 0;

	int infile = -1, //verify that these do infact go into the right file descriptors
		outfile = -1;

	int pd[2];

	//Fork process
	pid = fork();
	if (pid < 0) {
		printf("Fork error: %s\n", strerror(errno));
		//Error?
	}

	if(pid) { //Parent will be second command
		//Wait for child to terminate

		pid = wait(&r);
		//print exit status code]
		if(r != 0) {
			printf("Child died with status: %d\n", r);			
			printf("Error: %s\n", strerror(errno));
		}
		
		/*if(infile >= 0) {
			printf("closing infile\n");
			close(infile);
		}
		if(outfile >= 0) {
			printf("closing outfile\n");
			close(outfile);
		}*/
		//printf("Error: %s\n", strerror(errno));
		//return to main loop
	} else { //child
		printf("This is child\n");

		char* command = (char*)malloc(sizeof(char) * 128);


		for(i = 0; i < numArgs; i++) {
			if(strcmp(args[i], "<") == 0) { //input
				if(firstIO < 0) {
					firstIO = i;
				}
				
				i++;
				//open infile for readfil
				printf("infile: %s\n", args[i]);
				infile = open(args[i], O_RDONLY);

				if(infile < 0) {
					printf("File error: %s\n", strerror(errno));
				} else {
					printf("about to close 0\n");
					r = close(0);
					printf("close r = %d\n", r);
					dup2(infile, 0);
					/*printf("about to close 0: %d\n", close(0));
					printf("in fd: %d\n", infile);
					dup2(infile, 0);*/
				}
			} else if(strcmp(args[i], ">") == 0) { //overwriting
				if(firstIO < 0) {
					firstIO = i;
				}

				//open outfile for writing

				i++;
				printf("outfile: %s\n", args[i]);
				outfile = open(args[i], O_CREAT|O_WRONLY, 0666);

				if(outfile < 0) {
					printf("File error: %s\n", strerror(errno));
				} else {
					printf("about to close 1\n");
					r = close(1);
					printf("close r = %d\n", r);
					dup2(outfile, 1);
				}
				
			} else if(strcmp(args[i], ">>") == 0) { //appending
				if(firstIO < 0) {
					firstIO = i;
				}
				
				i++;
				//open outfile for appending
				outfile = open(args[i], O_APPEND|O_WRONLY|O_CREAT, 0666);

				if(outfile < 0) {
					printf("File error: %s\n", strerror(errno));
				} else {
					printf("about to close 1\n");
					r = close(1);
					printf("close r = %d\n", r);
					dup2(outfile, 1);
				}
				
			}
		}

		if(firstIO > 0) {
			//If there is any I/O redirection, adjust the array so the program doesn't get confused by those inputs
			args[firstIO] = NULL;
			numArgs = firstIO;
		}


		//execute command (will be in '/bin/')		
		strcpy(command, "/bin/");
		strcat(command, args[0]);
		//printf("command: %s\n", command);
		//fprintf(stderr, "Prog1: \n");
		execve(command, args, enviro);



		if(infile >= 0) {
			printf("closing infile\n");
			close(infile);
		}
		if(outfile >= 0) {
			printf("closing outfile\n");
			close(outfile);
		}
		
		printf("Error: %s\n", strerror(errno));
	}
}

int executeCMDs(char* cmds[], int numCommands) {
	//For each command
		//create array of char pointers to the arguments, pass to 
		//call execute CMD

	char* args[64] = {0};
	char* args2[64] = {0};
	char* token = (char*)malloc(sizeof(char) * 128);

	int i = 0,
		j = 0,
		a1 = 0,
		a2 = 0;

	token = strtok(cmds[i],  " ");
	j = 0;
	while(token != NULL) {
		args[j] = strdup(token);
		j++;
		a1++;
		token = strtok(NULL, " ");
	}
	if(numCommands == 2) {
		i++;
		token = strtok(cmds[i], " ");
		j = 0;
		while(token != NULL) {
			args2[j] = strdup(token);
			j++;
			a2++;
			token = strtok(NULL, " ");

		}
	}
	executeCMD(args, args2, a1, a2);

	/*for(; i < numCommands; i++) {
		printf("Command: %s\n", cmds[i]);
		token = strtok(cmds[i], " ");
		//args[0] = (char*)malloc(sizeof(char) * 128);
		//strcpy(args[0], token);
		//args[0] = token;
		j = 0; //the first command is the filename
		while(token != NULL) {
			args[j] = strdup(token);
			//args[j] = token;
			j++;
			a1++;
			
			token = strtok(NULL, " ");
		}
		args[j] = NULL;
		//executeCMD(args, args2j);
		memset(args, 0, 64);
	}*/
}