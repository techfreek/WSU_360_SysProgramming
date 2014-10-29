#include "type.h"
#include "utility.h"
#include "proc.h"
#include "fileops.h"

char line[128], cmd[32], pathname[64];
extern PROC *running;
extern MINODE *root;

int main() {
	fs_init();   // initialize FS data structures

	while(1){ 
		// ask for a command LINE; extract cmd, pathname from line:

		fgets(line, 128, stdin);
		line[strlen(line)-1] = 0;  // kill the \n char at end

		if (line[0]==0) {
			continue;
		}

		if (!strcmp(cmd, "ls")) {
			//list_file(pathname);
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