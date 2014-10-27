#include "mount.h"

MOUNT* mounttable[NMOUNT];

int init() {
	MOUNT* newDisk;

	for(int i = 0; i < NMOUNT; i++) {
		mounttable = NULL;
		/*newDisk = (MOUNT *)malloc(sizeof(MOUNT));		
		if(newDisk == NULL) {
			return 0; //Couldn't allocate, so we give up
		}
		

		/* sets all fields to empty */
		newDisk->dev = 0;
		newDisk->nblocks = 0;
		newDisk->ninodes = 0;
		newDisk->bmap = 0;
		newDisk->imap = 0;
		newDisk->iblk = 0;
		newDisk->mounted_inode = NULL;
		newDisk->name[0] = 0;
		newDisk->mount_name[0] = 0;
		
		mounttable[i] = newDisk;*/
	}
	return 1;
}

int mountDisk(int fd, MINODE *mounted_inode, char name[64], char mount_name[64]) {
	//Find first empty slot
	int i = 0;
	for(; i < NMOUNT && mounttable[i] != NULL; i++);
	
	//Opens disk, and stores the field in the first open slot
	char buf[BLKSIZE];
	SUPER *sp = NULL;
	GD *gp = NULL;

	mounttable[i] = (MOUNT*)malloc(sizeof(MOUNT));

	mounttable[i]->dev = fd; //store the file descriptor
	mounttable[i]->mounted_inode = mounted_inode; /* i need to look up where I get this field from */
	mounttable[i]->name = strdup(name);
	mounttable[i]->mount_name = strdup(mount_name);

	get_block(fd, 1, buf);
	sp = (SUPER *)buf;
	if(!isEXT2(sp->s_magic)) {
		printf("Not an EXT2 file system\n");
		return 0;
	}

	mounttable[i]->ninodes = sp->s_inodes_count;
	mounttable[i]->nblocks = sp->s_blocks_count;
	

	get_block(fd, 2, buf);
	gp = (GD *)buf;
	
	mounttable[i]->bmap = gp->bg_block_bitmap;
	mounttable[i]->imap = gp->bg_inode_bitmap;
	mounttable[i]->iblk = gp->bg_inode_table;

	//returns index + 1
	return i + 1;
}

int unMountDisk(int disk) {
	/* Work on this more, for right now, it will just clear it from memory and nothing more */
	//MOUNT* unmount = mounttable[disk - 1];
	free(mounttable[disk-1]->mounted_inode); //frees data held by the inode
	free(mounttable[disk-1]); //frees the memory held by the mount struct
	return 1;
}

int getFD(int disk) {
	return mounttable[disk - 1]->dev;

}

MINODE* getMountedInode(int disk) {
	return mounttable[disk - 1]->mounted_inode;
}

char* getName(int disk) {
	return mounttable[disk - 1]->name;
}

char* getMountName(int disk) {
	return mounttable[disk - 1]->mount_name;
}

int getNBlocks(int disk) {
	return mounttable[disk - 1]->nblocks;
}

int getNInodes(int disk) {
	return mounttable[disk - 1]->ninodes;
}

int getBMap(int disk) {
	return mounttable[disk - 1]->bmap;
}

int getIMap(int disk) {
	return mounttable[disk - 1]->imap;
}

int getIBlk(int disk) {
	return mounttable[disk - 1]->iblk;
}
