#include "source.h"

int isEXT2(u32 magic) {
	if (magic != 0xEF53) {
		printf("NOT an EXT2 FS\n");
		return 0;
	} else {
		return 1;
	}
}

int get_block(int fd, int blk, char buf[]) {
	lseek(fd, (long)blk*BLKSIZE, 0);
	read(fd, buf, BLKSIZE);
}

/* used to look up the block via the inode, currently it doesn't do anything */
int get_inode(int fd, int ino, int startInoTable, INODE* node) {
	char buf[BLKSIZE];
	int blockNum = ((ino - 1) / 8) + startInoTable;
	int block = (ino - 1) % 8;
	get_block(fd, blockNum, buf);
	node = (INODE*)buf + block; //Skip past the nodes we don't need
}

void printSuper() {
	// read SUPER block
	get_block(fd, 1, buf);  
	sp = (SUPER *)buf;

	if (!isEXT2(sp->s_magic != 0xEF53)) {
		printf("NOT an EXT2 FS\n");
		exit(1);
	}

	printf("EXT2 FS OK\n");

	printf("s_inodes_count = %d\n", sp->s_inodes_count);
	printf("s_blocks_count = %d\n", sp->s_blocks_count);

	printf("s_free_inodes_count = %d\n", sp->s_free_inodes_count);
	printf("s_free_blocks_count = %d\n", sp->s_free_blocks_count);
	printf("s_first_data_blcok = %d\n", sp->s_first_data_block);

	printf("s_log_block_size = %d\n", sp->s_log_block_size);

	printf("s_blocks_per_group = %d\n", sp->s_blocks_per_group);
	printf("s_inodes_per_group = %d\n", sp->s_inodes_per_group);

	printf("s_mnt_count = %d\n", sp->s_mnt_count);
	printf("s_max_mnt_count = %d\n", sp->s_max_mnt_count);

	printf("s_magic = %x\n", sp->s_magic);

	printf("s_mtime = %s", ctime(&sp->s_mtime));
	printf("s_wtime = %s", ctime(&sp->s_wtime));
}

/* gets first block of root */
int firstIBlock(int fd) {
	char buf[BLKSIZE];

	// read GD
	get_block(fd, 2, buf);
	gp = (GD *)buf;

	iblock = gp->bg_inode_table;   // get inode start block#
	printf("inode_block=%d\n", iblock);

	// get inode start block     
	get_block(fd, iblock, buf);

	ip = (INODE *)buf + 1;         // ip points at 2nd INODE
	
	return ip->i_block[0];
}

int search(int fd, int inoStart, char names[64][128], int dirsRemaining, int ino) {
	/*
		Logic:
		[x] Read block
		[x] Scans through each of the files in the block to see if's name matches
			[x] if it matches and is a file, and it's the file we are looking for
				[x] return it's ino
			[x] else its a match, but it's a file and it's not the file we are looking for (dirsRemaining == 0)
				[x] return 0
			[x] else if it's a directory and it's the next one on the path
				[x] return search(names+1, dirsRemaing - 1, ino of current dir)
			[x] //If we get out of the loop, the path is invalid
				[x] return 0
	*/

	char buf[BLKSIZE];
	char temp[BLKSIZE];
	char *cp;

	DIR   *dp; 
	INODE *parent;
	INODE *file;
	get_inode(fd, ino, parent);
	
	get_block(fd, parent->i_block[0]), buf);
	dp = (DIR *)buf;
	cp = buf;

	int match = 0;

	while(cp < (buf + 1024)) {
		strncpy(temp, dp->name, dp->name_len);
		temp[dp->name_len] = '\0';
		/*printf("Inode: %d, rec_len: %d, name_len: %d, name: %s \n", 
			dp->inode, dp->rec_len, dp->name_len, temp);*/

		match = isStrEq(names[0], temp)

		if(match) {
			get_inode(fd, dp->inode, inoStart, file);

			if(dirsRemaining == 0 && S_ISREG(file->i_mode)) {
				return dp->inode;
			} else if(dirsRemaining == 0 && !S_ISREG(file->i_mode)) {
				return 0;
			} else if(S_ISDIR(file->i_mode)) {
				return search(fd, inoStart, names + 1, dirsRemaining - 1, dp->inode);
			}
		}

		cp += dp->rec_len;
		dp = (SHUTUP)cp;        // pull dp along to the next record
	}

	printf("File not found\n");
	return 0;

}

int getIno(int fd, int inoStart, char *path) {
	/*
		[x] Tokenize path 
		[x] search
		[x] return search results
	*/
	char names[64][128];
	int numNames, rootDirBlock, targetINO = 0;

	//rootDirBlock = 	firstIBlock();

	numNames = tokenize(path, names);

	targetINO = search(fd, inoStart, names, numNames, 2);

	return targetINO;
}

int tokenize(char *path, char names[64][128]) {
	int i = 0;
	char *temp;

	if(strlen(path) == 0) { //Make sure I don't pass in nothing and crash
		return  0;
	} else {
		temp = strtok(path, "/");
		printf("First token: %s\n", temp);

		/* May need to be added if the first output is empty because it starts with /
			temp = strtok(NULL, "/");
		*/

		while(temp != NULL) {
			strcpy(names[i++], temp);
			temp = strtok(NULL, "/");
		}
		return i + 1; //returns total number of levels to traverse
	}
}

int isStrEq(const char* str1, const char* str2) {
	if(strcmp(str1, str2) == 0) {
		return 1;
	} else {
		return 0;
	}
}

void printBlocks(int fd, INODE* file) {
	int i = 0,
		end = 0;
	char buf[BLKSIZE];
	

	printf("Direct Blocks\n");
	for(; i < 12 && file->i_block[i] != NULL; i++) {
		printf("%d  ", file->i_block[i]);
	}

	//getchar();
	if(file->i_block[12] != 0) {
		printf("\n\nIndirect Blocks\n");
		get_block(fd, i_block[12], buf);
		printBlock(buf);
	}

	if(file->i_block[13] != 0) {
		char dblIBuf[BLKSIZE];
		int dblIBlock = 0;
		printf("\n\nDouble Indirect Blocks\n");
		get_block(fd, i_block[13], buf);

		for(i = 0; i < (BLKSIZE / 4) && !end; i++) {
			dblIBlock = (int)buf[i * 4];
			if(dblIBlock > 0) {
				get_block(fd, dblIBlock,  dblIBuf);
				printBlock(dblIBuf);
			} else {
				end++;
			}
		}
	}

	if(file->i_block[14] != 0) {
		end = 0;
		int doubleEnd = 0;
		int tripleIBlock = 0;
		int j = 0;
		char tripleIBuf[BLKSIZE];
		printf("\n\nTriple Indirect Blocks\n");
		get_block(fd, i_block[14], buf); //gets block containing references to blocks that have references to the actual blocks

		for(i = 0; i < (BLKSIZE / 4) && !end; i++) {
			dblIBlock = (int)buf[i * 4];
			if(dblIBlock > 0) {
				get_block(fd, dblIBlock,  dblIBuf);

				for(j = 0; j < (BLKSIZE / 4) && !doubleEnd; j++) {
					tripleIBlock = (int)dblIBuf[j * 4];
					if(tripleIBlock > 0) {
						get_block(fd, tripleIBlock, tripleIBuf);
						printBlock(tripleIBuf);
					} else {
						doubleEnd++;
					}
				}
				doubleEnd = 0;
			} else {
				end++;
			}
		}
	}
}

void printBlock(char buf[]) {
	int i = 0, end = 0, val = 0;
	for (i = 0; i < (BLKSIZE / 4) && !end; i++)
	{
		val = (int)buf[(i * 4)];
		if(val > 0) {
			printf("%d  ", val);
		} else {
			end++;
		}
	}
}