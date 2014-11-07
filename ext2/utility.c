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

	printf("Opening the disk for O_RDWR\n");
	int rootDiskFD = open(pathname, O_RDWR);

	if(!rootDiskFD) { 
		printf("Could not open the specied disk: %s", pathname);
		exit(1);
	}

	printf("Initializing Minode Table...\n");
	initMINODE();

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

	printf("Mounting root...\n");
	int rootID = mountDisk(fd, NULL, NULL, NULL); // Nulls are temporary until I learn what these fields are for
	root = iget(rootID, 2);

	printf("Creating inital procs\n");
	PROC *P0 = newProc(0);
	PROC *P1 = newProc(1);

	printf("Initializing P0 CWD\n");
	P0->cwd = iget(rootID, 2);

	printf("Initializing P1 CWD\n");
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
	char buf[BLKSIZE];
	get_block(devId, getBlockNum(ino, devId), buf);
	return (INODE*)buf + getBlock(ino, devId);//Skip past the nodes we don't need
}

int getBlockNum(int ino, int devId) {
	//Mail mans algorithm!
	return ((ino - 1) / 8) + getIBlk(devId);
}

int getBlock(int ino, int devId) {
	return (ino - 1) % 8;
}

void put_inode(int devId, int ino, INODE *linode) {
	//Get block
	//Overwrite where inode should be
	//write back
	INODE backup = *linode;
	char buf[BLKSIZE];
	char *cp;
	int blockNum = getBlockNum(ino, devId);
	int block = getBlock(ino, devId);

	get_block(devId, blockNum, buf);
	INODE *dInode = (INODE*)buf + block;
	
	//printf("Putting INODE: \n");
	//printInode(&backup);

	*dInode = backup; //copy values over

	//printf("Updated INODE: \n");
	//printInode(dInode);

	put_block(devId, blockNum, buf);
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
	char *copy = strdup(path);
	char *temp = NULL;

	char *bNameExists = strchr(path, '/'); //so we don't return the dir name as the basename
	if(!bNameExists) {
		return copy;
	} else {
		char *end = strrchr(copy, '/');
		if(end != NULL) {
			end++; //skip past the '/'
			//*end = 0; //null terminate at specified character
			return end;
		} else {
			return NULL;
		}
	}
}

char* bbasename(const char* path) {
	char* copy = strdup(path);
	char* temp = NULL;
	char* basename = (char*)malloc(NNAME);

	if(strlen(path) == 0) { //Make sure I don't pass in nothing and crash
		return 0;
	} else {
		char *bNameExists = strchr(path, '/'); //so we don't return the dir name as the basename

		if(bNameExists) {
			char *end = strrchr(copy, '/');

			strncpy(basename, copy, (end-copy));

			free(copy);
		}
		return basename; //returns total number of levels to traverse
	}
}

int getino(int devId, int startIno, char *pathname) {
	/*
		[x] Tokenize path 
		[x] search
		[x] return search results
	*/
	char names[64][128];
	int numNames, rootDirBlock, targetINO = 0;
	//INODE* file;
	//rootDirBlock = 	firstIBlock();

	if(pathname[0] == '/') { //start from root
		startIno = 2;
	}

	numNames = tokenize(pathname, names);

	targetINO = search(devId, names, numNames - 1, startIno);

	//file = getInode(disk, targetINO);

	//printBlocks(disk->fd, get_inode(disk, targetINO));

	return targetINO;
}

int searchMIP(MINODE *mip, char *name) {
	printf("Search by MIP is not implemented yet\n");
	return 0;
}

int search(int devId, char names[64][128], int dirsRemaining, int pIno) {
	char buf[BLKSIZE];

	INODE *parent;
	INODE *file;

	parent = get_inode(devId, pIno);
	int i = 0, tIno = 0;

	for(i = 0; i < 12 && !tIno && parent->i_block[i]; i++) {
		getchar();
		get_block(devId, parent->i_block[i], buf);
		tIno = findName(buf, names[0]);
	}

	if(!dirsRemaining) { //if we at the end of the search
		return tIno;
	} else {
		file = get_inode(devId, tIno);
		if(S_ISDIR(file->i_mode)) {
			getchar();
			return search(devId, names + 1, dirsRemaining -1, tIno);
		} else {
			return 0;
		}
	}
}

int findName(char buf[], char name[]) {
	char temp[BLKSIZE];
	char *cp;
	DIR   *dp; 

	dp = (DIR *)buf;
	cp = buf;
	while(cp < (buf + 1024)) {
		getDIRFileName(dp, temp);
		printf("name: %s Inode: %d, rec_len: %d, name_len: %d \n", 
			temp, dp->inode, dp->rec_len, dp->name_len);
		//getchar();
		//printf("%s = %s ? = %d\n", name, temp, strEq(name, temp));
		if(strEq(name, temp)) {
			return dp->inode;
		}

		cp += dp->rec_len;
		dp = (SHUTUP)cp;        // pull dp along to the next record
	}

	return 0; //Coudn't be found
}

int ialloc(int devId) {
	int  i;
	char buf[BLKSIZE];

	// read inode_bitmap block
	get_block(devId, getIMap(devId), buf);

	for (i=0; i < getNInodes(devId); i++){
		if (tst_bit(buf, i)==0){
			set_bit(buf,i);
			updateFreeInodes(devId, 1);

			put_block(devId, getIMap(devId), buf);

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
	get_block(devId, getBMap(devId), buf);

	for (i=0; i < getNBlocks(devId); i++){
		if (tst_bit(buf, i)==0){
			set_bit(buf,i);
			updateFreeBlocks(devId, 1);

			put_block(devId, getBMap(devId), buf);

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
	get_block(devId, getIMap(devId), buf);
			 
	clr_bit(buf,ino - 1);
	updateFreeInodes(devId, -1);

	put_block(devId, getIMap(devId), buf);

	return 0;
}

int bdealloc(int devId, int bno) {
	int  i;
	char buf[BLKSIZE];

	// read block_bitmap block
	get_block(devId, getBMap(devId), buf);
			 
	clr_bit(buf, bno - 1); //Account for indexing starting at 1
	updateFreeBlocks(devId, -1);

	put_block(devId, getBMap(devId), buf);
	
	return 0;
}

int strEq(const char* str1, const char* str2) {
	return (strcmp(str1, str2) == 0);
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
	get_block(devId, 1, buf);
	sp = (SUPER *)buf;
	sp->s_free_inodes_count + delta;
	put_block(devId, 1, buf);

	get_block(devId, 2, buf);
	gp = (GD *)buf;
	gp->bg_free_inodes_count + delta;
	put_block(devId, 2, buf);
}

//Pass 1 to increase by 1, -1 to decrease by 1, etc
int updateFreeBlocks(int devId, int delta) {
	char buf[BLKSIZE];

	// update free block count in SUPER and GD
	get_block(devId, 1, buf);
	sp = (SUPER *)buf;
	sp->s_free_blocks_count + delta;
	put_block(devId, 1, buf);

	get_block(devId, 2, buf);
	gp = (GD *)buf;
	gp->bg_free_blocks_count + delta;
	put_block(devId, 2, buf);
}

void printInode(INODE* ip) {
	printf("\n********** INODE Stats **********\n");
	
	printf("I_Mode  |  uid  |  size  |     atime     |  gid  |  links  \n");
	printf("%8d| %5d | %6d | %13d | %5d | %d\n", ip->i_mode, ip->i_uid, ip->i_size, ip->i_atime, ip->i_gid, ip->i_links_count);

	printf("**********  End INODE  **********\n\n");
}
