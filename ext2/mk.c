#include "mk.h"

extern PROC *running;
extern MINODE *root;

int mycreat(char *path) {
	printf("make_file\n");
	char *bname = bbasename(path);
	char *name = bdirname(path);
	int ino = running->cwd->ino; //by default the ino is the current dir
	if(bname) {
		ino = getino(getDevID(running->cwd->dev), running->cwd->ino, bname);
	}

	if(strlen(name) == 0) {
		printf("Please enter a file name\n");
	}
	
	MINODE *parent = iget(getDevID(running->cwd->dev), ino);

	printf("New File parent:\n");
	printMINode(parent);
	if(S_ISDIR(parent->INODE.i_mode)) {
		printf("got parent of to-be directory\n");
		if(create(parent, name)) { //mymkdir will return 1 on success
			parent->dirty++;

			touch(parent);
		}
	} else {
		printf("Invalid path\n");
	}
	iput(parent);
	return;
}

int make_dir(char *path) {
	printf("make_dir\n");
	char *bname = bbasename(path);
	char *name = bdirname(path);
	printf("dirname: %s\n", name);
	printf("Basename: %s\n", bname);
	int ino = running->cwd->ino; //by default the ino is the current dir
	if(bname) {
		ino = getino(getDevID(running->cwd->dev), running->cwd->ino, bname);
	}
	
	if(strlen(name) == 0) {
		printf("Please enter a name\n");
		return 0;
	}

	MINODE *parent = iget(getDevID(running->cwd->dev), ino);

	printf("New File parent:\n");
	printMINode(parent);

	printf("New dir name: %s \n", name);

	if(S_ISDIR(parent->INODE.i_mode) && !childExists(parent, name)) {
		printf("got parent of to-be directory\n");
		if(mymkdir(parent, name)) { //mymkdir will return 1 on success
			parent->dirty++;
			parent->INODE.i_links_count++;

			touch(parent);
		}
	} else {
		printf("Invalid path\n");
	}

	iput(parent);
	return;
}

int mymkdir(MINODE *parent, char *name) {
	/*
		add name to parent
		create inode (allocate inode)
	*/
	printf("Creating a dir on dev %d devId %d\n", parent->dev, getDevID(parent->dev));
	int nino = ialloc(getDevID(parent->dev));
	int nbno = balloc(getDevID(parent->dev));

	if(!nino || !nbno) {
		printf("No INOs or BNOs remaining.\n");
		iput(parent);
		return 0;
	}

	printf("Making new minode\n");

	MINODE *nChild = dupMINODE(parent); //So I get basic settings copied over
	nChild->ino = nino;
	
	printf("Initiazing new dir with inode %d\n", nino);

	INODE *cInode = get_inode(getDevID(nChild->dev), nino);
	cInode->i_block[0] = nbno;
	cInode->i_mode = DIR_MODE;
	cInode->i_uid = running->uid;
	cInode->i_size = (u32)BLKSIZE;	
	cInode->i_atime = time(NULL);
	cInode->i_ctime = time(NULL);
	cInode->i_mtime = time(NULL);
	cInode->i_dtime = time(NULL);
	cInode->i_gid = running->gid;
	cInode->i_links_count = 2;
	nChild->INODE = *cInode;

	//insertChild(nChild, nChild, ".");
	//insertChild(nChild, parent, "..");
	initDir(nbno, getDevID(nChild->dev), parent->ino, nino);

	printf("Directory i block: %d\n", nbno);

	nChild->dirty++;



	//put_inode(getDevID(nChild->dev), nino, cInode);

	insertChild(parent, nChild, name);
	iput(nChild);

	printf("DIR created\n");
	return 1;
}

int create(MINODE *parent, char *name) {
	/*
		add name to parent
		create inode (allocate inode)
	*/
	printf("Creating a file on dev %d devId %d\n", parent->dev, getDevID(parent->dev));
	int nino = ialloc(getDevID(parent->dev));
	int nbno = balloc(getDevID(parent->dev));

	if(!nino || !nbno) {
		printf("No INOs or BNOs remaining.\n");
		iput(parent);
		return 0;
	}

	printf("Making new minode\n");

	MINODE *nChild = dupMINODE(parent); //So I get basic settings copied over
	nChild->ino = nino;
	
	printf("Initiazing new file with inode %d\n", nino);

	INODE *cInode = get_inode(getDevID(nChild->dev), nino);
	cInode->i_block[0] = (u32)nbno;
	cInode->i_mode = FILE_MODE;
	cInode->i_uid = (u32)running->uid;
	cInode->i_size = (u32)0;	
	cInode->i_atime = (u32)time(NULL);
	cInode->i_ctime = (u32)time(NULL);
	cInode->i_mtime = (u32)time(NULL);
	cInode->i_dtime = (u32)time(NULL);
	cInode->i_gid = (u32)running->gid;
	cInode->i_links_count = 1;

	nChild->INODE = *cInode;

	nChild->dirty++;



	//put_inode(getDevID(nChild->dev), nino, cInode);

	insertChild(parent, nChild, name);
	iput(nChild);

	printf("File created\n");
	return 1;
}

//This scans through all i_blocks to see if the specified name exists
//Returns 1 if it does
//Returns 0 if it does not
int childExists(MINODE  *parent, char *childname) {
	int i = 0, ino = 0;
	INODE pInode = parent->INODE;
	char buf[BLKSIZE];	

	for(; i < SINGLEINDIRECT && pInode.i_block[i]; i++) {
		get_block(getDevID(parent->dev), pInode.i_block[i], buf);
		if(ino = findName(buf, childname)) {
			return ino;
		}
	} 
	return 0;
}

int initDir(int bno, int devId, int pino, int ino) {
	DIR *dot = (DIR *)malloc(sizeof(DIR)),
		*dotdot = (DIR *)malloc(sizeof(DIR)),
		*dp;

	char buf[BLKSIZE];
	char *cp;

	get_block(devId, bno, buf);
	cp = buf;

	dot->inode = ino;
	dot->rec_len = calcIdeal(1);
	dot->name_len = 1;
	strncpy(dot->name, ".", 1);

	dotdot->inode = pino;
	dotdot->rec_len = BLKSIZE - (dot->rec_len);
	dotdot->name_len = 2;
	strncpy(dotdot->name, "..", 2);

	dp = (SHUTUP)buf;
	*dp = *dot;
	cp += dot->rec_len;

	dp = (SHUTUP)cp;
	*dp = *dotdot;

	put_block(devId, bno, buf);

}

int insertChild(MINODE *parent, MINODE *child, char *name) {
	/*
		Insert child DIR struct into parent blocks
	*/

	int i = 0,
		devId = getDevID(parent->dev),
		oldLength = BLKSIZE,
		new_name_len = strlen(name);

	INODE *ip = &parent->INODE;
	char buf[BLKSIZE];
	char *cp, *np;
	DIR *dp, *nDir = (DIR *)malloc(sizeof(DIR));



	nDir->inode = child->ino;
	nDir->rec_len = calcIdeal(new_name_len);
	nDir->name_len = new_name_len;
	strncpy(nDir->name, name, new_name_len);

	for(; (i < SINGLEINDIRECT) && (ip->i_block[i] != 0); i++) {
		get_block(devId, ip->i_block[i], buf);
		cp = getLastDir(buf);
		
		dp = (DIR *)cp;
		if(!isIdeal(dp) && ((dp->rec_len - calcIdeal(dp->name_len)) >= nDir->rec_len)) {
			//printf("Found spot in i_block[%d]\n", i);
			oldLength = dp->rec_len;
			//printf("DP size before: %d\n", dp->rec_len);
			dp->rec_len = calcIdeal(dp->name_len);
			//printf("DP size after: %d\n", dp->rec_len);
			
			np = cp + dp->rec_len; //now points at where the new record will go
			
			//printf("Now writing new entry\n");

			nDir->rec_len = oldLength - dp->rec_len; //take up all the space left
			
			DIR *temp = (SHUTUP)np;
			*temp = *nDir;

			printf("old Dir entry: %d %d %d %s\n", dp->inode, dp->rec_len, dp->name_len, dp->name);
			printf("New Dir entry: %d %d %d %s\n", temp->inode, temp->rec_len, temp->name_len, temp->name);

			printf("Done writing new entry\n");

			put_block(devId, ip->i_block[i], buf);

			return;
		}
		
	}
	if(i == SINGLEINDIRECT) {
		printf("Simply amazing. I was able to fill up all direct blocks of a directory with DIR structs\n");
		return 0;	
	}

	//No space before we ran out of partial/complete i_blocks
	int nBlock = balloc(devId);
	if(nBlock) {
		printf("Creating new block for parent\n");
		parent->dirty++;
		parent->INODE.i_block[i] = nBlock; //new block!

		get_block(devId, nBlock, buf);

		nDir->rec_len = BLKSIZE; //We will use the whole block
		DIR *temp = (SHUTUP)buf;
		*temp = *nDir; //copy values over;

		put_block(devId, nBlock, buf); //write it back
	} else {
		printf("Sorry, couldn't add that file\n");
		return 0;
	}
}

char *getLastDir(char buf[]) {
	char *cp, *pp;
	DIR *dp;

	dp = (DIR *)buf;
	cp = buf;

	while(cp < (buf + 1024)) {
		pp = cp; //advance previous pointer
		cp += dp->rec_len;
		dp = (SHUTUP)cp;
	}
	return pp;
}

int isIdeal(DIR *dp) {
	int ideal = calcIdeal(dp->name_len);
	if(dp->rec_len == ideal) {
		return 1;
	} else {
		return 0;
	}
}

//Length passed in must not include null terminator
int calcIdeal(int len) {
	return 4 * ((8 + len + 3) / 4);
}
