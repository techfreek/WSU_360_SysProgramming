#include "minode.h"
MINODE* minodes[NMINODES];

/*
	INODE INODE;               // disk inode
	int   dev, ino;
	int   refCount;
	int   dirty;
	int   mounted;
	struct mount *mountptr;
*/


//ensures all are zero to start
void init() {
	for(int i = 0; i < NMINODES; i++) {
		minodes[i]->INODE = NULL;
		minodes[i]->dev = 0;
		minodes[i]->ino = 0;
		minodes[i]->refCount = 0;
		minodes[i]->dirty = 0;
		minodes[i]->mounted = 0;
		minodes[i]->mountptr = 0;
	}
}

MINODE *iget(int dev, int ino) {
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
}

int findmyname(MINODE *parent, int myino, char *myname) {
	/*
		Given the parent DIR (MINODE pointer) and myino, this function finds 
		the name string of myino in the parent's data block. This is the SAME
		as SEARCH() by myino, then copy its name string into myname[ ].
	*/
}

int findino(MINODE *mip, int *myino, *parentino) {
	/*
		For a DIR Minode, extract the inumbers of . and .. 
		Read in 0th data block. The inumbers are in the first two dir entries.
	*/
}
