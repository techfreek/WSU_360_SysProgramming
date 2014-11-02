#include "type.h"
#include "utility.h"
#include "proc.h"
#include "fileops.h"
#include "minode.h"
#include "rm.c"
#include "mk.c"

char line[128], cmd[32], pathname[64];
extern PROC *running;
extern MINODE *root;
char *diskimage = "fdimage";

int main() {
	fs_init(diskimage);   // initialize FS data structures

	while(1){ 
		// ask for a command LINE; extract cmd, pathname from line:


		printf("Command: ");
		fgets(line, 128, stdin);
		line[strlen(line)-1] = 0;  // kill the \n char at end

		getCMD(line);
		getPath(line);

		if (line[0]==0) {
			continue;
		}

		printf("CMD: %s PATH: %s\n", cmd, pathname);
		if(strEq(cmd, "quit") || strEq(cmd, "q")) {
			quit();
			exit(0);
		}

		int func = functionLookup(cmd);
		if(!func) {
			printf("bash: %s: command not found\n", cmd);
		} else {
			execcmd(func, pathname);
		}
	}
}

int getCMD(char *line) {
	char* copy = strdup(line); //ensure we don't mess up the original
	strcpy(cmd, strtok(copy, " "));
	free(copy);
	return;
}

int getPath(char *line) {
	char* copy = strdup(line); //ensure we don't mess up the original
	strtok(copy, " "); //This will be the command, ignore 
	char* temp = strtok(NULL, " ");

	if(temp) {
		strcpy(pathname, temp);
	} else {
		pathname[0] = 0; //Make the string empty if there is no path
	}

	free(copy); //free unneeded memory
}

int functionLookup(char *cmd) {
	char *cmds[] = {"ls", "pwd", "cd", "mkdir", "rmdir", "creat", "procs", "minodes", "mounts", 0};
	int i = 0;

	while(cmds[i] != NULL) {
		if(!strcmp(cmd, cmds[i])) {
			return i + 1;
		}
		i++;
	}
	return 0;
}

int execcmd(int cmd, char *path) {
	cmd--;
	
	void (*fptr[])(char*) = {ls, pwd, cd, mymkdir, myrmdir, mycreat, printAllProcs, printAllMINodes, printAllMounts};

	(*fptr[cmd])(path);
}
