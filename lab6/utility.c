#include "utility.h"

int fs_init() {
	/*
		(1). 2 PROCs, P0 with uid=0, P1 with uid=1, all PROC.cwd = 0, fd[]=0
		(2). PROC *running -> P0 -> P1 -> P0; (i.e. a circular link list)
		(3). MINODE minode[100]; all with refCount=0, dev=0, ino=0
		(5). MINODE *root = 0;
	*/
}

int mount_root() {
	/*
		dev = fd = open device for RW

		read SUPER block to verify it's an EXT2 FS;
			get nblocks, ninodes; SAVE them AS GLOBALs
 
		read GD to get block numbers of bmap,imap,inodes_start; save as globals
 
		root = iget(dev, 2);    // get root inode

		(OPTIONAL): initialize mount table[0] for the mounted root device.
		Since we already saved ninode, nblocks, bmap, imap, inodes_start as 
		globals, it is NOT necessary to repeat them in mount[0].
    
		Let cwd of both P0 and P1 point at the root minode (refCount=3)
			P0.cwd = iget(dev, 2); 
			P1.cwd = iget(dev, 2);
     */
}

int quit() {
	//Close all MINODES, and save those that are dirty
}

void get_block(int dev, int blk, char buf[]) {

}

void put_block(int dev, int blk, char buf[]) {

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

char* dirname(const char* path) {
	char* copy = strdup(path);
	char* temp = NULL;
}

char* basename(const char* path) {
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

int getino(int dev, char *pathname) {
	/*
		[x] Tokenize path 
		[x] search
		[x] return search results
	*/
	char names[64][128];
	int numNames, rootDirBlock, targetINO = 0;
	//INODE* file;
	//rootDirBlock = 	firstIBlock();

	numNames = tokenize(path, names);

	targetINO = search(disk, names, numNames - 1, 2);

	//file = getInode(disk, targetINO);

	//printBlocks(disk->fd, get_inode(disk, targetINO));

	return targetINO;
}

int search(MINODE *mip, char *name) {

}

int ialloc(int dev) {

}

int balloc(int dev) {

}

int idealloc(int dev, int ino) {

}

int bdealloc(int dev, int bno) {

}

int strEq(const char* str1, const char* str2) {
	return !(strcmp(str1, str2) == 0));
}