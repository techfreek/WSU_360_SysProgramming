#ifndef mk_c
#define mk_c

#include "type.h"
#include "utility.h"
#include "proc.h"
#include "fileops.h"
#include "minode.h"

int mycreat(char *path) {

}

int mymkdir(char *path) {

}

int doesChildExist(MINODE  *parent, char *childname) {
	int i = 0;
	INODE pInode = parent->INODE;
	char buf[BLKSIZE];	

	for(; i < SINGLEINDIRECT && pInode.i_block[i]; i++) {
		get_block(getDevID(parent->dev), pInode.i_block[i], buf);
		if(findName(buf, childname)) {
			return 1;
		}
	} 
	return 0;
}

int insertChild(MINODE *parent, MINODE *child) {
	
}

#endif
