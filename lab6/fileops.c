#include "fileops.h"

int ls(char *pathname) {
	/*int     ino = getino(dev, pathname);
	MINODE *mip = iget(dev,ino);

	if (mip->INODE is a file)
		list_file(mip, basename(pathname));
	else
		list_dir(mip);

	iput(mip);*/
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
	INODE *ip = &mip->INODE;
	//File type
	if(S_ISREG(ip->i_mode)) {
		putchar('-');
	} else if(S_ISDIR(ip->i_mode)) {
		putchar('d');
	} else if(S_ISLNK(ip->i_mode)) {
		putchar('l');
	}

	//Owner Privileges
	S_IRUSR(ip->i_mode) ? putchar('r') : putchar('-');
	S_IWUSR(ip->i_mode) ? putchar('w') : putchar('-');
	S_IXUSR(ip->i_mode) ? putchar('x') : putchar('-');

	//Group Privileges
	S_IRGRP(ip->i_mode) ? putchar('r') : putchar('-');
	S_IWGRP(ip->i_mode) ? putchar('w') : putchar('-');
	S_IXGRP(ip->i_mode) ? putchar('x') : putchar('-');

	//Other Privileges
	S_IROTH(ip->i_mode) ? putchar('r') : putchar('-');
	S_IWOTH(ip->i_mode) ? putchar('w') : putchar('-');
	S_IXOTH(ip->i_mode) ? putchar('x') : putchar('-');

	printf("%3s %3d %3d %6d %s %s", ip->i_links_count, ip->i_gid, ip->i_uid, ip->i_size, ctime(&ip->ctime), name);
	if(S_ISLNK(ip->i_mode)) {
		printf(" -> %s\n", (char *)ip->i_block);
	} else {
		putchar('\n');
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
			printf("%d %d %d %s\n", dp->inode, dp->rec_len, dp->name_len, name);
			cp += dp->rec_len;
			dp = (SHUTUP)cp;
		}
	}
} 

int getDIRFileName(DIR* dp, char buf[]) {
	strncpy(buf, dp->name, (dp->name_len - 1));
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
}

void pwd() {
	//print the pathname of running PROC's CWD
}

void rpwd() {
	//recursive step of pwd
	//if root, return
	//then on return, print '/' then filename
}


STAT* stat() {  // THIS IS TRIVIAL, hence no need to implement
	ino = getino(dev, pathname);
	mip = iget(dev, ino);
	copy info from minode.INODE to a stat structure.
}        