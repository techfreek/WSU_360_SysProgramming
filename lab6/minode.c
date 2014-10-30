#include "minode.h"
MINODE* minodes[NMINODES] = {NULL};

extern PROC *running;
MINODE *root = NULL;

//ensures all are zero to start
void initMINODE() {
	int i;
	for(i = 0; i < NMINODES; i++) {
		MINODE *newMINODE = (MINODE*)malloc(sizeof(MINODE));
		minodes[i] = newMINODE;
		//minodes[i]->INODE = NULL;
		minodes[i]->dev = 0;
		minodes[i]->ino = 0;
		minodes[i]->refCount = 0;
		minodes[i]->dirty = 0;
		minodes[i]->mounted = 0;
		minodes[i]->mountptr = 0;
	}
}

MINODE *iget(int devId, int ino) {
	/*
		Once you have the ino of an inode, you may load the inode into a slot
		in the Minode[] array. To ensure uniqueness, you must search the Minode[] 
		array to see whether the needed INODE already exists:
		 
		If you find the needed INODE already in a Minode[] slot, just inc its 
		refCount by 1 and return the Minode[] pointer.

		If you do not find it in memory, you must allocate a FREE Minode[i], load
		the INODE from disk into that Minode[i].INODE, initialize the Minode[]'s
		other fields and return its address as a MINODE pointer,
	*/

	//printf("Getting ino = %d on disk = %d\n", ino, devId);

	int i = 0, firstOpenSlot = -1;
	for(i = 0; i < NMINODES; i++) {
		if(minodes[i]->ino == ino) { //File is already loaded
			//printf("MINODE found!\n");
			break;
		}
		if(minodes[i]->ino == 0 && (firstOpenSlot < 0)) { //If we haven't found an open slot, and it's open, store the index
			firstOpenSlot = i;
			//printf("\nFirst open MINODE slot: %d\n", firstOpenSlot);
		}
	}

	//printf("i = %d\n", i);

	if(i == NMINODES) { //we searched the entire array
		//printf("Ino = %d was not found in minode table\n", ino);
		char buf[BLKSIZE];
		INODE *ip;
		//printf("Getting inode for ino %d\n", ino);
		ip = get_inode(devId, ino);
		MINODE* mip = minodes[firstOpenSlot];
		mip->INODE = *ip; //Copy over IP content
		mip->dev = getFD(devId);
		mip->refCount = 1;
		mip->ino = ino;
		mip->dirty = 0;
		//printf("Getting mount pointer for disk %d\n", devId);
		mip->mountptr = getMountPtr(devId);
		//printf("Got disk ptr\n");
		return mip;
	} else { //File is already on there
		//printf("%d users are currently connected to: %d\n", minodes[i]->refCount++, minodes[i]->ino);
		//minodes[i]->refCount++;
		
		return minodes[i];
	}
}

int iput(MINODE *mip) {
	/*
		This function releases a Minode[]. Since an Minode[]'s refCount indicates
		the number of users on this Minode[], releasing is done as follows:
		 
		First, dec the refCount by 1. 
		If (after dec) refCount > 0 ==> return;
		if Minode[].dirty == 0 ==> no need to write back, so return;
		--------------------------------------------------------------
		Otherwise, (dirty==1) ==> must write the INODE back to disk.
		 
		To write an INODE back to disk:
		 Use Minode's (dev, ino) to determine which dev and which INODE on disk,
		i.e. use Mailman's algorithm to determine disk block and which inode in that 
		block.
		Read that block in, copy Minode's INODE into the INODE area in that block
		and write the block back to disk.
	*/

	int i = 0, found = 0;
	for(i = 0; i < NMINODES; i++) {
		if((minodes[i]->dev == mip->dev) && (minodes[i]->ino == minodes[i]->ino)) { //we found our MINODE
			found++;
			break;
		}
	}
	if(found) {
		if(minodes[i]->refCount > 1) { //don't delete, just decrement refcount
			minodes[i]->refCount--;
			printf("%d other users are still using ino = %d\n", minodes[i]->refCount, minodes[i]->ino);
		} else {
			if(minodes[i]->dirty) {
				//write all back
			}
			//clear all data
			//minodes[i]->INODE = NULL;
			minodes[i]->mountptr = minodes[i]->dev = minodes[i]->ino = minodes[i]->refCount = minodes[i]->dirty = minodes[i]->mounted = 0; //clear ints
			//Just set pointer to null cause we don't want to delete that pre-emptively
		}
	}
	return;
}

int findmyname(MINODE *parent, int myino, char *myname) {
	/*
		Given the parent DIR (MINODE pointer) and myino, this function finds 
		the name string of myino in the parent's data block. This is the SAME
		as SEARCH() by myino, then copy its name string into myname[ ].
	*/



}

int findino(MINODE *mip, int myino, int parentino) {
	/*
		For a DIR Minode, extract the inumbers of . and .. 
		Read in 0th data block. The inumbers are in the first two dir entries.
	*/
}

void printMINode(MINODE *mip) {
	printf("\n########## MINODE Stats ##########\n");
	printf("Dev |  ino  | refCount | dirty | mounted \n");
	printf("%3d | %5d | %8d | %5d | %d\n", mip->dev, mip->ino, mip->refCount, mip->dirty, mip->mounted);
	printInode(&mip->INODE);

	printf("\n##########  End MINODE  ##########\n\n");
}

void printAllMINodes() {
	int i;
	MINODE *curr;
	printf("\n########## MINODEs ##########\n");
	for (i = 0; i < NMINODES; ++i)
	{
		if(minodes[i]->ino) { //it's valid
			printf("i   |Dev |  ino  | refCount | dirty | mounted \n");
			printf("%3d |%3d | %5d | %8d | %5d | %d\n", curr->dev, curr->ino, curr->refCount, curr->dirty, curr->mounted);
		}
	}
	printf("\n##########  End MINODEs  ##########\n\n");
}

void closeAll() {
	//clears Minode table and
	//Writes back all dirty blocks

}
