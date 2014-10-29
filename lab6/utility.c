#include "utility.h"

extern PROC *running;
extern MINODE *root;

int fs_init(char* pathname) {
	/*	
		(1). 2 PROCs, P0 with uid=0, P1 with uid=1, all PROC.cwd = 0, fd[]=0
		(2). PROC *running -> P0 -> P1 -> P0; (i.e. a circular link list)
		(3). MINODE minode[100]; all with refCount=0, devId=0, ino=0
		(5). MINODE *root = 0;
	*/

	initMINODE();
	
	int rootDiskFD = open(pathname, O_RDWR);

	if(!rootDiskFD) { 
		printf("Could not open the specied disk: %s", pathname);
		exit(1);
	}

	mount_root(rootDiskFD);

	printMINode(root);

	return rootDiskFD;
}

int mount_root(int fd) {
	/*
		root = iget(devId, 2);    // get root inode
		most of this is handled by mountDisk()
		Let cwd of both P0 and P1 point at the root minode (refCount=3)
			P0.cwd = iget(devId, 2); 
			P1.cwd = iget(devId, 2);
    */

	PROC *P0 = newProc(0);
	PROC *P1 = newProc(1);

	int rootID = mountDisk(fd, NULL, NULL, NULL); // Nulls are temporary until I learn what these fields are for
	root = iget(rootID, 2);
	P0->cwd = iget(rootID, 2);
	P1->cwd = iget(rootID, 2);
	printAllProcs();

}

int quit() {
	//Close all MINODES, and save those that are dirty
	closeAll();
	killAllProcs();
	unmountAll();
	//printf("Quit not implemented yet\n");
}

void get_block(int devId, int blk, char buf[]) {
	lseek(getFD(devId), (long)blk*BLKSIZE, 0);
	read(getFD(devId), buf, BLKSIZE);
}

void put_block(int devId, int blk, char buf[]) {
	//reverses the get_block algorithm
	lseek(getFD(devId), (long)blk*BLKSIZE, 0);
	write(getFD(devId), buf, BLKSIZE);
}

/* used to look up the block via the inode */
INODE* get_inode(int devId, int ino) {
	char buf[100];
	int blockNum = ((ino - 1) / 8) + getIBlk(devId);
	int block = (ino - 1) % 8;
	get_block(getFD(devId), blockNum, buf);
	return (INODE*)buf + block; //Skip past the nodes we don't need
}

int tokenize(char *path, char names[64][128]) {
	int i = 0;
	char *temp;

	if(strlen(path) == 0) { //Make sure I don't pass in nothing and crash
		return  0;
	} else {
		temp = strtok(path, "/");

		while(temp != NULL) {
			printf("Token[%d]: %s\n", i, temp);
			strcpy(names[i++], temp);
			temp = strtok(NULL, "/");
		}
		printf("Length of tokenized path: %d\n", i);
		return i; //returns total number of levels to traverse
	}
}

char* bdirname(const char* path) {
	char* copy = strdup(path);
	char* temp = NULL;
	printf("Dirname() not implemented yet\n");
}

char* bbasename(const char* path) {
	char* copy = strdup(path);
	char* temp = NULL;
	char* basename = NULL;

	if(strlen(path) == 0) { //Make sure I don't pass in nothing and crash
		return 0;
	} else {
		temp = strtok(copy, "/");

		while(temp != NULL) {
			basename = temp;
			temp = strtok(NULL, "/");
		}
		free(copy);
		return basename; //returns total number of levels to traverse
	}
}

int getino(int devId, char *pathname) {
	/*
		[x] Tokenize path 
		[x] search
		[x] return search results
	*/
	char names[64][128];
	int numNames, rootDirBlock, targetINO = 0;
	//INODE* file;
	//rootDirBlock = 	firstIBlock();

	numNames = tokenize(pathname, names);

	targetINO = search(devId, names, numNames - 1, 2);

	//file = getInode(disk, targetINO);

	//printBlocks(disk->fd, get_inode(disk, targetINO));

	return targetINO;
}

int searchMIP(MINODE *mip, char *name) {
	printf("Search by MIP is not implemented yet\n");
	return 0;
}

int search(int devId, char names[64][128], int dirsRemaining, int ino) {
	/*
		Logic:
		Read block
		Scans through each of the files in the block to see if's name matches
			if it matches and is a file, and it's the file we are looking for
				return it's ino
			else its a match, but it's a file and it's not the file we are looking for (dirsRemaining == 0)
				return 0
			else if it's a directory and it's the next one on the path
				return search(names+1, dirsRemaing - 1, ino of current dir)
			//If we get out of the loop, the path is invalid
				return 0
	*/

	char buf[BLKSIZE];
	char temp[BLKSIZE];
	char *cp;

	printf("\nSearching for %s, %d levels remain\n", names[0], dirsRemaining);

	DIR   *dp; 
	INODE *parent;
	INODE *file;
	parent = get_inode(devId, ino);
	printf("Got parent INODE\n");
	printInode(parent);
	
	get_block(devId, parent->i_block[0], buf);
	printf("Got parent's block\n");

	dp = (DIR *)buf;
	cp = buf;

	int match = 0;

	while(cp < (buf + 1024)) {
		strncpy(temp, dp->name, dp->name_len);
		temp[dp->name_len] = '\0';
		printf("name: %s Inode: %d, rec_len: %d, name_len: %d \n", 
			temp, dp->inode, dp->rec_len, dp->name_len);

		match = strEq(names[0], temp);

		if(match) {
			file = get_inode(devId, dp->inode);

			if(dirsRemaining == 0) {
				printf("Found file!\n");
				return dp->inode;
			} else if(S_ISDIR(file->i_mode)) {
				printf("Entering dir: %s\n", names[1]);
				return search(devId, names + 1, dirsRemaining - 1, dp->inode);
			} else {
				printf("Invalid path\n");
			}
		}

		cp += dp->rec_len;
		dp = (SHUTUP)cp;        // pull dp along to the next record
	}

	printf("File not found\n");
	return 0;

}

int ialloc(int devId) {
	int  i;
	char buf[BLKSIZE];

	// read inode_bitmap block
	get_block(getFD(devId), getIMap(devId), buf);

	for (i=0; i < getNInodes(devId); i++){
		if (tst_bit(buf, i)==0){
			 set_bit(buf,i);
			 updateFreeInodes(devId, 1);

			 put_block(getFD(devId), getIMap(devId), buf);

			 return i+1;
		}
	}
	
	printf("ialloc(): no more free inodes\n");
	return 0;
}

int balloc(int devId) {
	int  i;
	char buf[BLKSIZE];

	// read block_bitmap block
	get_block(getFD(devId), getBMap(devId), buf);

	for (i=0; i < getNBlocks(devId); i++){
		if (tst_bit(buf, i)==0){
			set_bit(buf,i);
			updateFreeBlocks(devId, 1);

			put_block(getFD(devId), getBMap(devId), buf);

			 return i+1;
		}
	}
	
	printf("balloc(): no more free inodes\n");
	return 0;
}

int idealloc(int devId, int ino) {
	int  i;
	char buf[BLKSIZE];

	// read inode_bitmap block
	get_block(getFD(devId), getIMap(devId), buf);
			 
	set_bit(buf,i);
	updateFreeInodes(devId, -1);

	put_block(getFD(devId), getIMap(devId), buf);

	return 0;
}

int bdealloc(int devId, int bno) {
	int  i;
	char buf[BLKSIZE];

	// read block_bitmap block
	get_block(getFD(devId), getBMap(devId), buf);
			 
	clr_bit(buf,i - 1); //Account for indexing starting at 1
	updateFreeBlocks(devId, -1);

	put_block(getFD(devId), getBMap(devId), buf);
	
	return 0;
}

int strEq(const char* str1, const char* str2) {
	return !(strcmp(str1, str2) == 0);
}

int isEXT2(u32 magic) {
	if (magic != 0xEF53) {
		printf("NOT an EXT2 FS\n");
		return 0;
	} else {
		return 1;
	}
}

int tst_bit(char *buf, int bit) {
	int i, j;
	i = bit/8; j=bit%8;
	if (buf[i] & (1 << j))
		 return 1;
	return 0;
}

int set_bit(char *buf, int bit) {
	int i, j;
	i = bit/8; j=bit%8;
	buf[i] |= (1 << j);
}

int clr_bit(char *buf, int bit) {
	int i, j;
	i = bit/8; j=bit%8;
	buf[i] &= ~(1 << j);
}

int updateFreeInodes(int devId, int delta) {
	char buf[BLKSIZE];

	// dec free inodes count in SUPER and GD
	get_block(getFD(devId), 1, buf);
	sp = (SUPER *)buf;
	sp->s_free_inodes_count + delta;
	put_block(getFD(devId), 1, buf);

	get_block(getFD(devId), 2, buf);
	gp = (GD *)buf;
	gp->bg_free_inodes_count + delta;
	put_block(getFD(devId), 2, buf);
}

//Pass 1 to increase by 1, -1 to decrease by 1, etc
int updateFreeBlocks(int devId, int delta) {
	char buf[BLKSIZE];

	// update free block count in SUPER and GD
	get_block(getFD(devId), 1, buf);
	sp = (SUPER *)buf;
	sp->s_free_blocks_count + delta;
	put_block(getFD(devId), 1, buf);

	get_block(getFD(devId), 2, buf);
	gp = (GD *)buf;
	gp->bg_free_blocks_count + delta;
	put_block(getFD(devId), 2, buf);
}

void printInode(INODE* ip) {
	printf("\n********** INODE Stats **********\n");
	
	printf("I_Mode  |  uid  |  size  |   atime   |  gid  |  links  \n");
	printf("%8d| %5d | %6d | %9d | %5d | %d\n", ip->i_mode, ip->i_uid, ip->i_size, ip->i_atime, ip->i_gid, ip->i_links_count);

	printf("**********  End INODE  **********\n\n");
}
