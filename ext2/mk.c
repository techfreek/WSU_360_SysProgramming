#include "mk.h"

extern PROC *running;
extern MINODE *root;

int mycreat(char *path) {
	char *bname = bbasename(path);
	int ino = getino(getDevID(running->cwd->dev), running->cwd->ino, bname);
	
	MINODE *parent = iget(ino, running->cwdDevId);

	if(S_ISDIR(parent->INODE.i_mode)) {
		printf("Creating a file\n");
		/*
			add name to parent
			create inode (allocate inode)
		*/
	} else {
		printf("Invalid path\n");
	}
	iput(parent);

}

int make_dir(char *path) {
	printf("make_dir\n");
	char *bname = bbasename(path);
	char *name = bdirname(path);
	int ino = running->cwd->ino; //by default the ino is the current dir
	if(bname) {
		ino = getino(getDevID(running->cwd->dev), running->cwd->ino, bname);
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
	
	printf("Initiazing new dir\n");

	INODE *cInode = get_inode(getDevID(nChild->dev), nChild->ino);
	cInode->i_block[0] = nbno;
	cInode->i_mode = DIR_MODE;
	cInode->i_uid = running->uid;
	cInode->i_size = BLKSIZE;	
	time(&cInode->i_atime);
	time(&cInode->i_ctime);
	time(&cInode->i_mtime);
	time(&cInode->i_dtime);
	cInode->i_gid = running->gid;
	cInode->i_links_count = 2;
	
	put_inode(getDevID(nChild->dev), nChild->ino, cInode);
	
	insertChild(parent, nChild, name);
	printf("Directory created\n");
	return 1;
}

//This scans through all i_blocks to see if the specified name exists
//Returns 1 if it does
//Returns 0 if it does not
int childExists(MINODE  *parent, char *childname) {
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
	DIR *dp, *nDir;



	nDir->inode = (u32)child->ino;
	nDir->rec_len = calcIdeal(new_name_len);
	nDir->name_len = new_name_len;
	strncpy(nDir->name, name, new_name_len);

	for(; (i < SINGLEINDIRECT) && (ip->i_block[i] != 0); i++) {
		get_block(devId, ip->i_block[i], buf);
		cp = getLastDir(buf);

		dp = (DIR *)cp;
		if(!isIdeal(dp) && ((dp->rec_len - calcIdeal(dp->name_len)) >= nDir->rec_len)) {
			oldLength = dp->rec_len;
			dp->rec_len = calcIdeal(dp->name_len);

			np = cp + dp->rec_len; //now points at where the new record will go
			
			printf("Now writing new entry\n");

			nDir->rec_len = oldLength - dp->rec_len; //take up all the space left
			//---------------------------------------------------np = (char)nDir;
			printf("Done writing new entry\n");

			put_block(devId, ip->i_block[i], buf);
			return;
		}
		
	}

	//No space before we ran out of partial/complete i_blocks
	int nBlock = balloc(devId);
	if(nBlock) {
		parent->dirty++;
		parent->INODE.i_block[i] = nBlock; //new block!

		get_block(devId, nBlock, buf);

		nDir->rec_len = BLKSIZE; //We will use the whole block
		//---------------------------------------------------buf = (char)nDir;

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
