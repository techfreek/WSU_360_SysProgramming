#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"

main() {
	initialize();      /* initialize the / DIR of the tree */
	while(1){
		printf(": "); //Add pwd to the prompt?
		gets(&line);
		parseLine(line);
		//read a line containting  command [pathname]; // [ ] means optional
		//Find the command string and call the corresponding function;
		//Example: 
		int ID = findCommand(command)
		if(ID >= 0) {
			int (*fptr[ ])(char *) = {(int (*)())mkdir,rmdir,ls, cd, pwd,creat,rm,save,reload};	
			int r = fptr[index](pathname);
		} else {
			printf("\"%s\" is not a valid command\n", command);
		}

		if(ID == 9) { //command = quit
			break;
		}
	}
}

void initialize() {
	root = (Node*)malloc(sizeof(Node));
	root->name = "/";
	root->type = 'D';
	root->childPtr = NULL;
	root->siblingPtr = &root;
	root->parentPtr = &root;
}

void parseLine(static char line[]) {
	&command = strtok(line, " ");
	if(command != NULL) {
		&pathname = strtok(NULL, " ");
	}
	
}

int findCommand(char command[]) {
	switch(command) {
		case "mkdir":
			return 0;
			break;
		case "rmdir":
			return 1;
			break;
		case "ls":
			return 2;
			break;
		case "cd":
			return 3;
			break;
		case "pwd":
			return 4;
			break;
		case "creat":
			return 5;
			break;
		case "rm":
			return 6;
			break;
		case "save":
			return 7;
			break;
		case "reload":
			return 8;
			break;
		case "quit":
			return 9;
			break;
	}
	return -1;
}