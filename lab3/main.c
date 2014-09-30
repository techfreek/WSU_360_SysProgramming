#include "source.h"

extern char* enviro[];
extern char* home;

int changeDir(char* path);
int handlePipes(char* line, char*[] commands);
int countPipes(char* line);

int main(int argc, char *argv[ ], char* env[]) {
	enviro = env;
	int exitStatus = 0;
	char* line = (char*)malloc(sizeof(char) * 512);
	char* cmd = (char*)malloc(sizeof(char) * 256);
	char* path = (char*)malloc(sizeof(char) * 256);
	char* commands[];

	do {
		printf("> ");
		scanf("%s", line);
		

		if(strncmp("cd", line, 2) == 0) {
			cmd = strtok(line, " "); //strtok occasionally destroys the string, so this is a failsafe
			path = strtok(NULL, " ");
			changeDir(path);
		} else if(strcmp("exit", line, 4) == 0) {
			exit(1);
		} else {
			int numCommands = handlePipes(line, commands);
			executeCMDs(commands, numCommands);
		}

	} while(exitStatus == 0);
	return 0;
}

int changeDir(char* path) {
	if(path != NULL && strlen(path) < 0) {
		chdir(path);
	} else {
		if(home == NULL) {
			int i = 0;
			for(; enviro[i] != 0 && strncmp(enviro[i], "HOME=", 5) != 0; i++); //should find the index of the element that is home
			printf("Found home: %s\n", enviro[i]);
			home = (enviro[i] + 5); //Should ignore the 'HOME=' part
			printf("Home: %s\n", home);
			
		}
		chdir(home);
	}
}

int handlePipes(char* line, char*[] commands) {
	int numPipes = countPipes(line),
		i = 0;

	char* temp = (char*)malloc(sizeof(char) * 128);
	commands = (char*[])malloc((sizeof(char) * 128) * numPipes + 1); //each pipe (max of numPipes + 1) 128 chars
	
	strtok(line, temp);
	if(numPipes > 0) {
		//cmd1 < infile //do this
		strcat(temp, "< infile");
		strcpy(commands[0], temp);
	} else {
		strcpy(commands[0], temp);
	}

	for(i = 1; i < numPipes; i++) {
		strtok(NULL, temp);
		if(numPipes > 1 && i != (numPipes - 1)) { //middleman pipes
			//do this:  cmd1 << infile > outfile	
			//strcat the < infile and > outfile onto temp
		} else if(i == (numPipes - 1)) { //last pipe
			//do this: cmdN < outfile
			//Cats the '< outfile' onto temp
		}
		strcat(commands[i], temp);
	}

}

int countPipes(char* line) {
	int i = 0,
		count = 0;
	for(i = 0; i < strlen(line); i++) {
		if(line[i] == '|') {
			count++;
		}
	}
	return count;
}