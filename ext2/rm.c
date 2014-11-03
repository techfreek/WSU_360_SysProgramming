#ifndef rm_c
#define rm_c

#include "proc.h"
#include "type.h"
#include "utility.h"
#include "fileops.h"
#include "minode.h"

extern PROC *running;
extern MINODE *root;

int myunlink(char *path) {
	/*int ino = getino(running->cwdDevId, running->cwd.ino, path);
	
	INODE *file = get_inode(running->cwdDevId, ino);

	if(S_ISREG(file->i_mode)) {
		
	} else {
		printf("Specified file (%s) is not a file.\n", path);
	}
	free(file);*/

	printf("Unlink not implemented yet\n");
	return;
}

int  myrmdir(char *path) {
	int ino = getino(running->cwdDevId, running->cwd.ino, path);
	
	INODE *dir = get_inode(running->cwdDevId, ino);
	
	printf("rmdir not completed yet\n");

	if(S_ISDIR(dir->i_mode)) {
		int links = dir->i_links_count;
		int busy = isActive(ino, running->cwdDevId);
		/*
			if links == 2 and !busy, 
				if directory is empty
					remove name from parents house
					decrase parents refcount
					free blocks
					free inode block
					???
					profit		 

		*/
	} else {
		printf("Specified directory (%s) is not a directory.\n", path);
	}
}

#endif
