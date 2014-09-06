#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"

main() {
	initialize();      /* initialize the / DIR of the tree */
	while(1){
		printf("input a commad : ");
		//read a line containting  command [pathname]; // [ ] means optional
		//Find the command string and call the corresponding function;
		//Example: 
		int ID = findCommand(command)
		int (*fptr[ ])(char *) = {(int (*)())mkdir,rmdir,ls, cd, pwd,creat,rm};
	}
}