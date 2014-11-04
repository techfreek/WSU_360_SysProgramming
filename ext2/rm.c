#ifndef rm_c
#define rm_c

#include "proc.h"
#include "type.h"
#include "utility.h"
#include "fileops.h"
#include "minode.h"
#include "mk.c"

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


int removechild(int ino, int devId) {
	int i = 0;
	INODE *ip = get_inode(devId, ino);
	char *cp, *pp;
	DIR *child;

	for(; (i < SINGLEINDIRECT) && (ip->i_block[i] != 0); i++) {
		get_block(devId, ip->i_block[i], buf);

		if(cp = inodeExists(ino, buf, &pp)) { //this is intentional so we don't have to scan again
			child = (SHUTUP)cp;
			/*
				if child size == 1024  -> delete block
				if child size > ideal  -> increase length of previous entry cover us
				if child size == ideal -> shift blocks over from the right and extend the length of the last entry

			*/


		}

	}
	printf("Unknown problem removing file\n");
}

char* inodeExists(int ino, char buf[], char **pp) {
	int i = 0;
	INODE pInode = parent->INODE;
	char buf[BLKSIZE];
	char *cp;
	DIR *dp;

	dp = (DIR *)buf;
	cp = buf;

	while(cp < (buf + 1024)) {
		pp = cp; //in case the first one is the one we are looking for
		if(dp->inode == ino) {
			return cp;
		}
		cp += dp->rec_len;
		dp = (SHUTUP)cp;
	}
	return NULL;
}


#endif
