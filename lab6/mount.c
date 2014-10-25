#include "mount.h"

MOUNT* mounttable[NMOUNT];

int init() {
	for(int i = 0; i < NMOUNT; i++) {
		/* sets all fields to empty */
	}
}

int mountDisk(int fd, MINODE *mounted_inode, char name[64], char mount_name[64]) {
	//Opens disk, and stores the field in the first open slot
	//returns index
}

int getFD(int disk) {
	return mounttable[disk]->dev;

}

MINODE* getMountedInode(int disk) {
	return mounttable[disk]->mounted_inode;
}

char* getName(int disk) {
	return mounttable[disk]->name;
}

char* getMountName(int disk) {
	return mounttable[disk]->mount_name;
}

int getNBlocks(int disk) {
	return mounttable[disk]->nblocks;
}

int getNInodes(int disk) {
	return mounttable[disk]->ninodes;
}

int getBMap(int disk) {
	return mounttable[disk]->bmap;
}

int getIMap(int disk) {
	return mounttable[disk]->imap;
}

int getIBlk(int disk) {
	return mounttable[disk]->iblk;
}
