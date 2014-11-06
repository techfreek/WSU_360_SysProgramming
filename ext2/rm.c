#include "rm.h"

extern PROC *running;
extern MINODE *root;

int myunlink(char *path) {
	/*int ino = getino(running->cwdDevId, running->cwd->ino, path);
	
	INODE *file = get_inode(running->cwdDevId, ino);

	if(S_ISREG(file->i_mode)) {
		
	} else {
		printf("Specified file (%s) is not a file.\n", path);
	}
	free(file);*/

	printf("Unlink not implemented yet\n");
	return;
}

int myrmdir(char *path) {
	char *bname = bbasename(path);
	char *name = bdirname(path);
	int ino = running->cwd->ino; //by default the ino is the current dir
	int devId = getDevID(running->cwd->dev);
	if(bname) {
		ino = getino(getDevID(running->cwd->dev), running->cwd->ino, bname);
	}
	
	MINODE *parent = iget(devId, ino);
	//MINODE *target = iget(devId, getino(devId, parent->ino, name));


	/*
		Refactor such that parent is the directory containing the directory we want to delete 
			ex if we want to delete a/b/c, parent is b, target is c
		etc
	*/


	//int childIno = childExists(parent, name);

	//INODE *childInode = get_inode(getDevID(parent->dev), childIno);

	if(S_ISDIR(parent->INODE.i_mode)) {
		int links = parent->INODE.i_links_count;
		int busy = isActive(ino, running->cwd->dev);
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
		if(links == 2 && !busy &&) { //see if we should continue
			if(parent->INODE.i_uid == SUPER_USER || parent->INODE.i_uid == running->uid)  {
				if(isEmptyDir(&parent->INODE, getDevID(parent->dev))) {
					clearBlocks(&parent->INODE, getDevID(parent->dev));
					idealloc(ino);


				} else {
					printf("Directory is not empty\n");
					return 0;
				}
			} else {
				printf("Insufficent permissions to delete file\n");
				return 0;
			}
		} else {
			printf("Could not delete directory\n");
			return 0;
		}
	} else {
		printf("Specified directory (%s) is not a directory.\n", path);
	}
}


int removechild(int ino, int devId) {
	int i = 0;
	INODE *ip = get_inode(devId, ino);
	char buf[BLKSIZE];
	char *cp, *pp;
	DIR *child, *prev;

	for(; (i < SINGLEINDIRECT) && (ip->i_block[i] != 0); i++) {
		get_block(devId, ip->i_block[i], buf);

		if(cp = inodeExists(ino, buf, &pp)) { //this is intentional so we don't have to scan again
			child = (SHUTUP)cp;
			prev = (SHUTUP)pp;
			/*
				if child size == 1024  -> delete block
				if child size > ideal  -> increase length of previous entry cover us
				if child size == ideal -> shift blocks over from the right and extend the length of the last entry

			*/
			if(child->rec_len == BLKSIZE) {
				//delete block
				bdealloc(devId, ip->i_block[i]);
				ip->i_block[i] = 0; //clear it so we don't think we still have it
			} else if(child->rec_len > calcIdeal(child->name_len)) {
				//extend length of previous
				prev->rec_len += child->rec_len;
			} else if(isIdeal(child)) {
				//shift blocks over, increase length of last entry
				DIR *last = (DIR*)getLastDir(buf);
				last->rec_len += child->rec_len;
				memcpy(cp, cp + child->rec_len, (BLKSIZE - child->rec_len));
			}
			return 1;
		}
	}
	printf("Unknown problem removing file\n");
}

char* inodeExists(int ino, char buf[], char **pp) {
	int i = 0;
	INODE pInode = parent->INODE;
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

int isEmptyDir(INODE *ip, int devId) {
	char buf[BLKSIZE];
	char *cp;
	DIR *dp;
	int first_rec_len = 0;

	get_block(devId, pInode.i_block[0], buf);
	cp = buf;
	dp = (SHUTUP)buf; //get '.'
	first_rec_len = dp->rec_len;

	cp += dp->rec_len;
	dp = (SHUTUP)buf;
	if(dp->rec_len + first_rec_len == BLKSIZE) {
		return 1;
	}
	return 0;
}

int clearBlocks(INODE *ip, int devId) {
	int i = 0;
	for(; i < SINGLEINDIRECT; i++) {
		if(ip->i_block[i]) {
			bdealloc(devId, ip->i_block[i]);
		}
	}
}