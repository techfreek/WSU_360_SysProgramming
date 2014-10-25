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