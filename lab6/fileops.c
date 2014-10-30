#include "fileops.h"

extern PROC *running;
extern MINODE *root;

int ls(char *pathname) {
	/*int     ino = getino(dev, pathname);
	MINODE *mip = iget(dev,ino);

	if (mip->INODE is a file)
		list_file(mip, basename(pathname));
	else
		list_dir(mip);

	iput(mip);*/
	int devId = running->cwdDevId;
	int ino;
	MINODE *mip;

	printf("LSing path: %s\n", pathname);
	if(strlen(pathname)) { //if a path is provided
		ino = getino(devId, pathname);	
		mip = iget(devId, ino);
	} else {
		ino = running->cwd->ino;
		mip = running->cwd;
	}
	
	if(S_ISREG(mip->INODE.i_mode)) {
		printf("Listing file\n");
		list_file(mip, bbasename(pathname));
	} else {
		list_dir(mip);
	}

	iput(mip); //close file
}

int list_file(MINODE* mip, char* name) {
	/*
	 INODE *ip = &mip->INODE;
							    R L D
	 use ip->i_mode to print  [-|l|d] rwxrwxrwx 
	 use ip->OTHER FIELDs to print:  link  gid  uid  size  date  name

	 if ip->i_mode is a LNK file:
			printf(" -> %s\n", (char *)ip->i_block);
			*/
	printMINode(mip);
	INODE *ip = &(mip->INODE);
	//File type

	printf("i_mode reg: %d dir: %d lnk: %d\n", S_ISREG(ip->i_mode), S_ISDIR(ip->i_mode), S_ISLNK(ip->i_mode));
	if(S_ISREG(ip->i_mode)) {
		printf("-");
	} else if(S_ISDIR(ip->i_mode)) {
		printf("d");
	} else if(S_ISLNK(ip->i_mode)) {
		printf("l");
	}

	//Owner Privileges
	(S_IRUSR & ip->i_mode) ? printf("r") : printf("-");
	(S_IWUSR & ip->i_mode) ? printf("w") : printf("-");
	(S_IXUSR & ip->i_mode) ? printf("x") : printf("-");

	//Group Privileges
	(S_IRGRP & ip->i_mode) ? printf("r") : printf("-");
	(S_IWGRP & ip->i_mode) ? printf("w") : printf("-");
	(S_IXGRP & ip->i_mode) ? printf("x") : printf("-");

	//Other Privileges
	(S_IROTH & ip->i_mode) ? printf("r") : printf("-");
	(S_IWOTH & ip->i_mode) ? printf("w") : printf("-");
	(S_IXOTH & ip->i_mode) ? printf("x") : printf("-");

	char time[NNAME];
	strcpy(time, ctime(&ip->i_ctime));
	time[strlen(time) - 1] = 0;

	printf("%3d %3d %3d %6d %s %s", ip->i_links_count, ip->i_gid, ip->i_uid, ip->i_size, time, name);
	if(S_ISLNK(ip->i_mode)) {
		printf(" -> %s\n", (char *)ip->i_block);
	} else {
		printf("\n");
	}
}

int list_dir(MINODE* mip) {
	/*MINODE *cip;
	INODE *ip = &mip->INODE;
	for each DIR entry in (data blocks of) *ip do{
		 // print [ino rec_len name_len name]
		 cip = iget(dev, ino);

		 list_file(cip, name);

		 iput(cip);
	}*/
	MINODE *cip;
	INODE *ip = &mip->INODE;
	DIR* dp;
	char buf[BLKSIZE];
	char name[128];
	char *cp;

	int i = 0, devId = getDevID(mip->dev);
	for(; (i < 12) && (ip->i_block[i] > 0); i++) { //If a directory needs to start using indirect blocks to store its files, something is wrong
		get_block(devId, ip->i_block[i], buf);
		dp = (DIR *)buf;
		cp = buf;

		while(cp < (buf + 1024)) {
			getDIRFileName(dp, name);
			printf("\t%4d %4d %2d    %s\n", dp->inode, dp->rec_len, dp->name_len, name);
			MINODE *mip = iget(devId, dp->inode);
			if(S_ISREG(mip->INODE.i_mode)) {
				list_file(mip, name);
			}
			iput(mip); //relase hold
			cp += dp->rec_len;
			dp = (SHUTUP)cp;
		}
	}
} 

int getDIRFileName(DIR* dp, char buf[]) {
	strncpy(buf, dp->name, (dp->name_len));
	buf[dp->name_len] = 0; //make sure we don't have a longer name that isn't overwritten
}

int cd(char* pathname) {
	/*if (no pathanme)
		cd to root;

		ino = getino(dev, pathname);
		mip = iget(dev, ino);

		VERIFY mip->INODE is a DIR;

		iput(running PROC's cwd);
		set running PROC's cwd to mip;
	 }*/
	if(!strlen(pathname)) {
		iput(running->cwd); //close file (don't leave it open for nothing)
		running->cwd = root;
	} else {
		int devId = running->cwdDevId;
		int ino = getino(devId, pathname);
		MINODE* mip = iget(devId, ino);

		if(S_ISDIR(mip->INODE.i_mode)) {
			iput(running->cwd); //close file (don't leave it open for nothing)
			running->cwd = mip;
		} else {
			printf("Specified path is invalid\n");
		}

	}
}

void pwd() {
	//print the pathname of running PROC's CWD
	if(running->cwd == root) {
		printf("/");
	} else {
		rpwd(running->cwd);
	}
	printf("\n");
}

MINODE* rpwd(MINODE *mip) {
	//recursive step of pwd
	//if root, return
	//then on return, print '/' then filename
	if(mip == root) {
		return mip; //return parent
	} else {
		MINODE* parent = rpwd(mip);
		char* name = (char*)malloc(sizeof(char) * NNAME);

		findmyname(parent, mip->ino, name);

		printf("/%s", name);
		free(name);
		return mip;
	}
}


/*STAT* stat() {  // THIS IS TRIVIAL, hence no need to implement
	ino = getino(dev, pathname);
	mip = iget(dev, ino);
	copy info from minode.INODE to a stat structure.
} */       
