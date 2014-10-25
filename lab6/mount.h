#ifndef mount_h
#define mount_h

/*
	int    dev;
	int    nblocks,ninodes;
	int    bmap, imap, iblk;
	MINODE *mounted_inode;
	char   name[64]; 
	char   mount_name[64];
*/

#include "types.h"

int mountDisk(int fd, MINODE *mounted_inode, char name[64], char mount_name[64]);
int getFD(int disk);
MINODE* getMountedInode(int disk);
char* getName(int disk);
char* getMountName(int disk);
int getNBlocks(int disk);
int getNInodes(int disk);
int getBMap(int disk);
int getIMap(int disk);
int getIBlk(int disk);


#endif