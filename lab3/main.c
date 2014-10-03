#include "source.h"

char** enviro;
char* home;

int changeDir(char* path);
int handlePipes(char* line, char* commands[], int numPipes);
int countPipes(char* line);

int main(int argc, char *argv[], char* env[]) {
	enviro = env;
	int exitStatus = 0;
	char* line = (char*)malloc(sizeof(char) * 512);
	char* cmd = (char*)malloc(sizeof(char) * 256);
	char* path = (char*)malloc(sizeof(char) * 256);
	char* buf = (char*)malloc(sizeof(char) * 128);

	do {
		printf("%s$ ", getcwd(buf, 128));
		gets(line);
		if(strncmp("cd", line, 2) == 0) {
			cmd = strtok(line, " "); //strtok occasionally destroys the string, so this is a failsafe
			path = strtok(NULL, " ");
			changeDir(path);
		} else if(strcmp("exit", line) == 0) {
			exit(1);
		} else if(strlen(line) > 0) {
			int numPipes = countPipes(line);
			char* commands[numPipes];
			handlePipes(line, commands, numPipes);
			executeCMDs(commands, numPipes);
		}

	} while(exitStatus == 0);
	return 0;
}

int changeDir(char* path) {
	if(path != NULL && strlen(path) > 0) {
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

int handlePipes(char* line, char* commands[], int numPipes) {
	int i = 0;

	char* temp;
	for(i = 0; i < numPipes; i++) {
		commands[i] = malloc(sizeof(char) * 128); //each pipe (max of numPipes + 1) 128 chars
	}
	
	temp = strdup(strtok(line, "|"));

	for(i = 0; i < numPipes; i++) {
		char string[32];
		
		/*if(i == 0 && numPipes > 1) { //Don't bother if we have only 1 command
			strcat(temp, " > io");
			sprintf(string, "%d", i);
			strcat(temp, string);
		} else if(i == (numPipes - 1) && numPipes > 1) { //last pipe
			strcat(temp, " < io");
			sprintf(string, "%d", i - 1);
			strcat(temp, string);
		} else if(numPipes > 2) { //middle man pipes
			strcat(temp, " < io");
			sprintf(string, "%d", i - 1);
			strcat(temp, string);
			strcat(temp, " > io");
			sprintf(string, "%d", i);
			strcat(temp, string);
		}*/
		strcat(commands[i], temp);

		
		if((i + 1) != numPipes) { //If we do at this point, it will crash
			temp = strdup(strtok(NULL, "|")); 	
		}
	}

}

int countPipes(char* line) {
	int i = 0,
		count = 1; //Starts from 1 because if the line is not empty, we will have at least 1 command
	for(i = 0; i < strlen(line); i++) {
		if(line[i] == '|') {
			count++;
		}
	}
	return count;
}

//Custom strtok function, use tok to indicate which token
char* stringtok(char* string, char tokens[], int tok) {
	int i = 0, j = 0, len = strlen(string);
	for(; i < len; i++) {
	}
}