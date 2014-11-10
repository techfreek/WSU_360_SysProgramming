#include "rm.h"

extern PROC *running;
extern MINODE *root;

int myrmdir(char *path) {
	char *bname = bbasename(path);
	char *name = bdirname(path);
	int ino = running->cwd->ino; //by default the ino is the current dir
	int devId = getDevID(running->cwd->dev);
	if(bname) {
		ino = getino(getDevID(running->cwd->dev), running->cwd->ino, bname);
	}
	
	MINODE *parent = iget(devId, ino);
	
	printInode(&parent->INODE);

	if(S_ISDIR(parent->INODE.i_mode)) { //see if we can move into it to find out more
		int tino = childExists(parent, name);
		if(!tino) {
			iput(parent);
			printf("Specified folder does not exist\n");
			return 0;
		} else {
			printf("Found child with ino: %d\n", tino);
		}

		INODE targetInode = *get_inode(devId, tino);
		if(!S_ISDIR(targetInode.i_mode)) { 
			printf("Specified item is not a directory\n");
			return 0;
		}
	
		list_file(&targetInode, "Directory to be deleted\n");
		printf("dir i_block[0] = %d\n", targetInode.i_block[0]);

		int links = targetInode.i_links_count;
		int busy = isActive(tino, running->cwd->dev);
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
		if(links == 2 && !busy) { //see if we should continue
			if(targetInode.i_uid == SUPER_USER || targetInode.i_uid == running->uid)  {
				//list_file(targetInode, "Directory to be deleted");
				//printf("dir i_block[0] = %d\n", targetInode->i_block[0]);
				if(isEmptyDir(&targetInode, devId)) {
					printf("Clearing blocks\n");
					clearBlocks(&targetInode, devId);
					printf("Deallocing inode\n");
					idealloc(devId, tino);
					printf("Removing from parent\n");
					removechild(parent, tino, devId);
					parent->INODE.i_links_count--;
					parent->dirty++;
				} else {
					printf("Directory is not empty\n");
					iput(parent);
					return 0;
				}
			} else {
				printf("Insufficent permissions to delete file\n");
				iput(parent);
				return 0;
			}
		} else {
			if(links != 2) printf("This directory has %d links\n", links);
			if(busy) printf("This directory is still being used\n");
			printf("Could not delete directory\n");
			iput(parent);
			return 0;
		}
	} else {
		printf("Specified directory (%s) is not a directory.\n", path);
	}
}


int removechild(MINODE *parent, int ino, int devId) {
	int i = 0;
	INODE *tp = get_inode(devId, ino);
	char buf[BLKSIZE];
	char *cp, *pp;
	DIR *child, *prev = NULL;

	for(; (i < SINGLEINDIRECT) && (parent->INODE.i_block[i] != 0); i++) {
		get_block(devId, parent->INODE.i_block[i], buf);
		int blocknum = parent->INODE.i_block[i];
		if(cp = inodeExists(parent, ino, buf, &pp)) { //this is intentional so we don't have to scan again
			child = (SHUTUP)cp;
			if(pp) {
				prev = (SHUTUP)pp;
			}
			/*
				if child size == 1024  -> delete block
				if child size > ideal  -> increase length of previous entry cover us
				if child size == ideal -> shift blocks over from the right and extend the length of the last entry

			*/
			if(child->rec_len == BLKSIZE) {
				//delete block
				printf("Deleting block\n");
				bdealloc(devId, ip->i_block[i]);
				ip->i_block[i] = 0; //clear it so we don't think we still have it
			} else if(child->rec_len > calcIdeal(child->name_len)) {
				//extend length of previous
				printf("Extending second to last block\n");
				prev->rec_len += child->rec_len;
			} else if(isIdeal(child)) {
				//shift blocks over, increase length of last entry
				printf("Shifting DIRs over\n");
				DIR *last = (DIR*)getLastDir(buf);
				last->rec_len += child->rec_len;
				char temp[BLKSIZE];
				char *np = cp + child->rec_len;
				int cpylen = (buf + BLKSIZE) - cp;
				//strncpy(temp, np, cpylen);
				//strncpy(cp, temp, cpylen);
				memcpy(cp, np, cpylen);
			}
			put_block(devId, blocknum, buf);
			list_dir(parent);
			return 1;
		}
	}
	printf("Unknown problem removing file\n");
}

char* inodeExists(MINODE *parent, int ino, char buf[], char **pp) {
	int i = 0;
	INODE pInode = parent->INODE;
	char *cp;
	DIR *dp;
	*pp = NULL;

	dp = (DIR *)buf;
	cp = buf;

	while(cp < (buf + 1024)) {
		if(dp->inode == ino) {
			return cp;
		}
		*pp = cp; 
		cp += dp->rec_len;
		dp = (SHUTUP)cp;
	}
	return NULL;
}

int isEmptyDir(INODE *ip, int devId) {
	char buf[BLKSIZE];
	char *cp;
	DIR *dp;
	int first_rec_len = 0;
	char name[NNAME];

	get_block(devId, ip->i_block[0], buf);
	cp = buf;
	dp = (SHUTUP)buf; //get '.'
	getDIRFileName(dp, name);
	printf("\t%4d %4d %2d    %s\n", dp->inode, dp->rec_len, dp->name_len, name);
	first_rec_len = dp->rec_len;

	cp += dp->rec_len;
	dp = (SHUTUP)cp;
	getDIRFileName(dp, name);
	printf("\t%4d %4d %2d    %s\n", dp->inode, dp->rec_len, dp->name_len, name);
	if(dp->rec_len + first_rec_len == BLKSIZE) {
		printf("Dir is empty\n");
		return 1;
	}
	return 0;
}

int clearBlocks(INODE *ip, int devId) {
	int i = 0;
	for(; i < SINGLEINDIRECT && ip->i_block[i]; i++) {
		bdealloc(devId, ip->i_block[i]);
	}
}
