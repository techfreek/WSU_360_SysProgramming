#include "link.h"

int link(char *old_file, char *new_file) {
	/*
		Verify that oldfile path is valid and not a folder
		Verify that nothing in basename(new_file) has the same name as dirname(new_file)
		Verify both paths are on the same device

		insert new DIR struct into basename(new_file)

		increment old_file's link count
		iput(old_file)

	*/
	char *oldbasename = bbasename(old_name);
	char *oldname = bdirname(old_name);
	char *newbasename = bbasename(new_file);
	char *newname = bdirname(new_file);

	int cino;
	int ino = running->cwd->ino;
	int devId = getDevId(running->cwd->dev);

	if(oldbasename) {
		ino = getino(devId, running->cwd, oldbasename);
	}

	MINODE *parent = iget(devId, ino);

	if(S_ISDIR(parent->INODE.i_mode)) {
		int tino = childExists(parent, oldname); //get folder we will link to
		if(!tino) {
			iput(parent);
			printf("Specified file does not exist\n");
			return 0;
		} else {
			printf("REG file found with ino %d\n", tino);
		}

		MINODE *original = iget(devId, tino);

		if(newbasename) {
			cino = getino(devId, running->cwd, newbasename);
		}

		MINODE *newParent = iget(devId, cino);
		if(S_ISDIR(newParent->INODE.i_mode)) {
			int nino = childExists(newParent, newname);
			if(!nino) {
				//we can create the hardlink
				insertChild(newParent, original, name);
				printf("Link created\n");
				original->INODE.i_links_count++;
				iput(original);
				iput(parent);
			}
		} else {
			iput(original);
			iput(parent);
			printf("Not a valid destination\n");
			return 0;
		}
	}

}

int symlink(char *old_name, char *new_file) {
	/*
		Make sure old_name < 59 (don't forget null byte)
		Verify that oldfile path is valid and not a folder
		Create file new_file with type LNK
		write old_name to i_block
		i put both
	*/
	char *oldbasename = bbasename(old_name);
	char *oldname = bdirname(old_name);
	char *newbasename = bbasename(new_file);
	char *newname = bdirname(new_file);

	if(strlen(old_name) >= 60) {
		printf("The original file's name is too long\n");
		return 0;
	}

	int ino = running->cwd->ino;
	int devId = getDevId(running->cwd->dev);

	if(oldbasename) {
		ino = getino(devId, running->cwd, oldbasename);
	}

	MINODE *parent = iget(devId, ino);

	if(S_ISDIR(parent->INODE.i_mode)) {
		int tino = childExists(parent, oldname);
		if(!tino) {
			printf("Specified file does not exist\n");
			return 0;
		} else {
			printf("REG file found with ino %d\n", tino);
		}

		MINODE *child = iget(devId, tino);
		if(S_ISDIR(child->INODE.i_mode)) {
			printf("You can't link to Dirs!\n");
			iput(parent);
			iput(child);
			return 0;
		} else {
			//create LNK file
				//creat() now returns the ino. So run iget(devid, creat(parent, name))
				//then change type to LNK
				//then write oldname to i_block
			//iput()
			int newParentIno;
			MINODE *newParent = parent; //default to same dir
			if(newbasename) {
				newParentIno = getino(devId, running->cwd, newbasename);
				newParent = iget(devId, newParentIno);
			}

			int lnkIno = create(newParent, newname);

			INODE *cInode = get_inode(devId, lnkIno);
			cInode->i_block = (char*)oldname;
			cInode->i_mode = 0120000; //Set LNK type... i think
			put_inode(devId, lnkIno, cInode);
			printf("Created SYMLNK\n");
			if(newParent != parent) {
				iput(newParent);
			}
			iput(parent);
			iput(child);
		}



	}

}

int unlink(char *pathname) {
	/*
		get pathname and get ino
		check if REG or LNK
		decement i_links_count by 1
		if links == 0
			deallocate all data blocks
			dalloc inode
		remove name from parent
	*/
	char *bname = bbasename(path);
	char *name = bdirname(path);

	int ino = running->cwd->ino;
	int devId = getDevId(running->cwd->dev);

	if(bname) {
		ino = getino(devId, running->cwd, bname);
	}

	MINODE *parent = iget(devId, ino);

	printInode(&parent->INODE);

	if(S_ISDIR(parent->INODE.i_mode)) {
		int tino = childExists(parent, name);
		if(!tino) {
			printf("Specified file does not exist\n");
			iput(parent);
			return 0;
		} else {
			printf("REG file found with ino %d\n", tino);
		}

		MINODE *child = iget(devId, tino);

		if(!--child->INODE.i_links_count) { //links count now 0, proceed to delete
			clearBlocks(&child->INODE, devId);
			idealloc(devId, tino);
			removechild(parent, tino, devId);
		} else {
			//don't mark as dirty if removing otherwise we'd try to write it back...
			child->dirty++;
		}

		iput(child);
		iput(parent);
	}

}