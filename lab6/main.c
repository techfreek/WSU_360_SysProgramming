#include "utility.h"

char LINE[128], cmd[32], pathanme[64]

int main() {
	init();   // initialize FS data structures

	mount_root();

	while(1){ 
		// ask for a command LINE; extract cmd, pathname from line:

		fgets(line, 128, stdin);
		line[strlen(line)-1] = 0;  // kill the \n char at end

		if (line[0]==0) {
			continue;
		}

		if (!strcmp(cmd, "ls") {
			list_file(pathname);
		} else if (!strcmp(cmd, "cd"){
			change_dir(pathname);
		} else if (!strcmp(cmd, "pwd") {
			pwd(running->cwd);
		} else if (!strcmp(cmd, "quit") {
			quit(); // write back any dirty in-memory INODE; exit(0);
		}
	}
}