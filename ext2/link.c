#include "link.h"

extern PROC *running;
extern MINODE *root;

int link(char *old_file, char *new_file) {
	/*
		Verify that oldfile path is valid and not a folder
		Verify that nothing in basename(new_file) has the same name as dirname(new_file)
		Verify both paths are on the same device

		insert new DIR struct into basename(new_file)

		increment old_file's link count
		iput(old_file)

	*/
	if(strlen(old_file) == 0 || strlen(new_file) == 0) {
		printf("Please provide a file name and a link file name\n");
		return 0;
	}

	char *oldbasename = bbasename(old_file);
	char *oldname = bdirname(old_file);
	char *newbasename = bbasename(new_file);
	char *newname = bdirname(new_file);

	int cino = running->cwd->ino;
	int ino = running->cwd->ino;
	int devId = getDevID(running->cwd->dev);

	if(oldname) {
		ino = getino(devId, running->cwd, oldname);
	}

	MINODE *parent = iget(devId, ino);

	if(S_ISDIR(parent->INODE.i_mode)) {
		int tino = childExists(parent, oldbasename); //get folder we will link to
		if(!tino) {
			iput(parent);
			printf("Specified file does not exist\n");
			return 0;
		} else {
			printf("REG file found with ino %d\n", tino);
		}

		MINODE *original = iget(devId, tino);

		if(newname) {
			cino = getino(devId, running->cwd, newname);
		}

		MINODE *newParent = iget(devId, cino);
		if(S_ISDIR(newParent->INODE.i_mode)) {
			int nino = childExists(newParent, newbasename);
			if(!nino) {
				//we can create the hardlink
				insertChild(newParent, original, newbasename);
				printf("Link created\n");
				original->INODE.i_links_count++;
				original->dirty++;
				printf("Original now has  %d links\n", original->INODE.i_links_count);
				iput(newParent);
				iput(original);
				iput(parent);
			} else {
				iput(newParent);
				iput(original);
				iput(parent);
			}
		} else {
			iput(newParent);
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
	printf("Symlnk\n");
	if(strlen(old_name) == 0 || strlen(new_file) == 0) {
		printf("Please provide a file name and a link file name\n");
		return 0;
	}



	char *oldbasename = bbasename(old_name);
	char *oldname = bdirname(old_name);
	char *newbasename = bbasename(new_file);
	char *newname = bdirname(new_file);


	if(strlen(old_name) >= 60) {
		printf("The original file's name is too long\n");
		return 0;
	}

	int ino = running->cwd->ino;
	int devId = getDevID(running->cwd->dev);

	if(oldname) {
		ino = getino(devId, running->cwd, oldname);
	}

	printf("Parent dirname: %s ino: %d\n", oldname, ino);
	printf("Oldbasename: %s\n", oldbasename);

	MINODE *parent = iget(devId, ino);

	printf("Got parent inode\n");

	if(S_ISDIR(parent->INODE.i_mode)) {
		int tino = childExists(parent, oldbasename);
		if(!tino) {
			iput(parent);
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
			int newParentIno = running->cwd->ino;
			MINODE *newParent = parent; //default to same dir
			if(newname) {
				printf("Searching new name\n");
				newParentIno = getino(devId, running->cwd, newname);
			}
			newParent = iget(devId, newParentIno);

			int lnkIno = create(newParent, newbasename);

			if(lnkIno) {
				INODE *cInode = get_inode(devId, lnkIno);
				char* i_name = &(cInode->i_block);
				strncpy(i_name, old_name, 60);
				cInode->i_mode = LNK_MODE; //Set LNK type
				put_inode(devId, lnkIno, cInode);
				printf("Created SYMLNK\n");
			} else {
				printf("Could not create link\n");
			}

			iput(newParent);
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
	if(strlen(pathname) == 0) {
		printf("Please provide a file name and a link file name\n");
		return 0;
	}

	char *bname = bbasename(pathname);
	char *name = bdirname(pathname);

	printf("unlink dirname: %s, basename: %s\n", name, bname);

	int ino = running->cwd->ino;
	int devId = getDevID(running->cwd->dev);

	if(name) {
		ino = getino(devId, running->cwd, name);
	}

	MINODE *parent = iget(devId, ino);

	printInode(&parent->INODE);

	if(S_ISDIR(parent->INODE.i_mode)) {
		int tino = childExists(parent, bname);
		if(!tino) {
			printf("Specified file does not exist\n");
			iput(parent);
			return 0;
		} else {
			printf("REG file (%s) found with ino %d\n", bname, tino);
		}

		MINODE *child = iget(devId, tino);
		if(!S_ISDIR(child->INODE.i_mode)) {
			child->INODE.i_links_count--;
			if(!child->INODE.i_links_count) { //linkks count now 0, proceed to delete
				//don't mark as dirty if removing otherwise we'd try to write it back...
				printf("Last one out, hit the lights\n");

				//blocks don't need to be cleared if it is a sym link... it has no blocks
				if(!S_ISLNK(child->INODE.i_mode)) {
					clearBlocks(&child->INODE, devId);
					printf("Blocks cleared\n");
				}
				idealloc(devId, tino);
				printf("Ino deallocated\n");
				removechild(parent, tino, devId);
				printf("Removed from parent\n");
			} else {
				printf("Removing name from parent. Other users connected to inode.\n");
				removechild(parent, tino, devId); //we want to remove the link with the specified path
				child->dirty++;
			}
		} else {
			printf("Silly, you can't unlink a dir!\n");
		}

		iput(child);
		iput(parent);
	}

}