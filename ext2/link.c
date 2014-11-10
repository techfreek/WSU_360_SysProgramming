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
		ino = getino(devId, running->cwd, bname);
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