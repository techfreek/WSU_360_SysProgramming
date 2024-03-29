#include "source.h"

void initialize();
void parseLine(char[]);
int findCommand(char[]);

 //Initialize external variables
Node* root;
Node* cwd;

char line[128] = "";
char command[16] = "";
char pathname[64] = "";
char dirname[64] = "";
char basename[64] = "";

main() {
	initialize();      /* initialize the / DIR of the tree */
	while(1){
		char* wd = getPWD(NULL);
		printf("User:%s$ ", wd);
		free(wd);
		gets(line);
		parseLine(line);
		//read a line containting  command [pathname]; // [ ] means optional
		int ID = findCommand(command);
		if(ID >= 0) {
			int (*fptr[ ])(char *) = {(int (*)())mkdir,rmdir,ls,cd,pwd,creat,rm,save,reload,quit};	
			int r = fptr[ID](pathname);
			if(ID == 9) { //command = quit
				break;
			}
		} else {
			printf("\"%s\" is not a valid command\n", command);
		}
	}
}

void parseLine(char line[]) {
	char copy[128] = "";
	char *temp = "";
	strcpy(copy, line);
	strcpy(command, strtok(copy, " "));
	temp = strtok(NULL, " ");
	if(temp != NULL) {
		strcpy(pathname, temp);
	} else {
		pathname[0] = '\0'; //reset to nothing
	}
}

int findCommand(char command[]) {
	//couldn't think of a better way to look up the files
	if(strcmp(command, "mkdir") == 0) {
		return 0;
	} else if(strcmp(command, "rmdir") == 0) {
		return 1;
	} else if(strcmp(command, "ls") == 0) {
		return 2;
	} else if(strcmp(command, "cd") == 0) {
		return 3;
	} else if(strcmp(command, "pwd") == 0) {
		return 4;
	} else if(strcmp(command, "creat") == 0) {
		return 5;
	} else if(strcmp(command, "rm") == 0) {
		return 6;
	} else if(strcmp(command, "save") == 0) {
		return 7;
	} else if(strcmp(command, "reload") == 0) {
		return 8;
	} else if(strcmp(command, "quit") == 0 || strcmp(command, "q") == 0 ) {
		return 9;
	} else {
		return -1;
	}
}