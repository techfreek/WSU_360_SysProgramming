#include "type.h"
#include "utility.h"
#include "proc.h"
#include "fileops.h"

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

		if (line[0]==0) {
			continue;
		}

		if (!strcmp(cmd, "ls")) {
			ls(pathname);
		} else if (!strcmp(cmd, "cd")) {
			cd(pathname);
		} else if (!strcmp(cmd, "pwd")) {
			pwd(running->cwd);
		} else if (!strcmp(cmd, "quit")) {
			quit(); // write back any dirty in-memory INODE; exit(0);
			exit(0);
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
	free(temp);


}