#include "mount.h"

MOUNT* mounttable[NMOUNT] = { NULL };

int mountDisk(int fd, MINODE *mounted_inode, char name[64], char mount_name[64]) {
	//Find first empty slot
	int i = 0;	
	for(; i < NMOUNT && mounttable[i] != NULL; i++);
	printf("First empty mount slot = %d\n", i);

	//Opens disk, and stores the field in the first open slot
	char buf[BLKSIZE];
	SUPER *sp = NULL;
	GD *gp = NULL;

	mounttable[i] = (MOUNT*)malloc(sizeof(MOUNT));

	mounttable[i]->dev = fd; //store the file descriptor
	mounttable[i]->mounted_inode = mounted_inode; /* i need to look up where I get this field from */
	if(name != NULL) {
		strncpy(mounttable[i]->name, name, NNAME);
	} 

	if(mount_name != NULL) {
		strncpy(mounttable[i]->mount_name, mount_name, NNAME);
	}

	printf("Reading SUPER block\n");

	get_block(i+1, 1, buf); //we use the index of the mount for this
	sp = (SUPER *)buf;
	if(!isEXT2(sp->s_magic)) {
		printf("Not an EXT2 file system\n");
		exit(0); //Just give up while you're ahead
	}

	mounttable[i]->ninodes = sp->s_inodes_count;
	mounttable[i]->nblocks = sp->s_blocks_count;
	
	printf("Reading GD block\n");
	get_block(i+1, 2, buf);
	gp = (GD *)buf;
	
	mounttable[i]->bmap = gp->bg_block_bitmap;
	mounttable[i]->imap = gp->bg_inode_bitmap;
	mounttable[i]->iblk = gp->bg_inode_table;

	printf("Disk mounted\n");

	//returns index + 1
	return i + 1;
}

int unMountDisk(int disk) {
	/* Work on this more, for right now, it will just clear it from memory and nothing more
		I should have it verify that it is a valid disk
		Then have it go and clean up any associated files on the minode table
	 */
	//MOUNT* unmount = mounttable[disk - 1];
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	free(mounttable[disk-1]->mounted_inode); //frees data held by the inode
	free(mounttable[disk-1]); //frees the memory held by the mount struct
	mounttable[disk-1] = NULL;
	return 1;
}

int unmountAll() {
	printf("Unmount all not implemented yet\n");
}

int getFD(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->dev;
}

int getDevID(int fd) { //In case we lost the devId but have the fd
	int i = 0;
	for(; i < NMOUNT; i++) {
		if(mounttable[i]->dev == fd) {
			return i + 1;
		}
	}
	return 0;
}

MINODE* getMountedInode(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->mounted_inode;
}

char* getName(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->name;
}

char* getMountName(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->mount_name;
}

int getNBlocks(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->nblocks;
}

int getNInodes(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->ninodes;
}

int getBMap(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->bmap;
}

int getIMap(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->imap;
}

int getIBlk(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk - 1]->iblk;
}

MOUNT* getMountPtr(int disk) {
	if(disk == 0) { printf("Invalid disk ID"); return 0; }
	return mounttable[disk -1];
}

void printAllMounts() {
	int i;
	for(i = 0; i < NMOUNT; i++) {
		if(mounttable[i] != NULL) {
			printMount(i + 1);
		}
	}
}

void printMount(int disk) {
	MOUNT *dp = mounttable[disk-1];
	printMountPtr(dp);
}

void printMountPtr(MOUNT* dp) {
	printf("\n@@@@@@@@@@ Mount Stats @@@@@@@@@@\n");
	printf("Name: %s", dp->name);
	printf("Mount Name: %s\n", dp->mount_name);

	printf("Device | nBlocks | nInodes | bmap | imap | iblk\n");
	printf("%6d | %7d | %7d | %4d | %4d | %4d\n", dp->dev, 
			dp->nblocks, dp->ninodes, dp->bmap, dp->imap, dp->iblk);
	printf("-----------------------------------------------\n");

	printf("@@@@@@@@@@  End Mount  @@@@@@@@@@\n\n");
}