#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

#define BLKSIZE 1024

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs
typedef struct DIR* SHUTUP; //tell the compiler to shut up


GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

int fd;
int iblock;

int get_block(int fd, int blk, char buf[]);
int dir();
int firstIBlock();

int get_block(int fd, int blk, char buf[]) {
	lseek(fd,(long)blk*BLKSIZE, 0);
	read(fd, buf, BLKSIZE);
}

int dir() {
	char buf[BLKSIZE];
	char temp[BLKSIZE];
	char *cp;

	int blockNum = firstIBlock();

	printf("ip->i_block[0] = %d\n", blockNum);

	get_block(fd, blockNum, buf);
	dp = (DIR *)buf;
	cp = buf;

	while(cp < (buf + 1024)) {
		strncpy(temp, dp->name, dp->name_len);
		temp[dp->name_len] = '\0';
		printf("Inode: %d, rec_len: %d, name_len: %d, name: %s \n", 
			dp->inode, dp->rec_len, dp->name_len, temp);
		cp += dp->rec_len;
		dp = (SHUTUP)cp;        // pull dp along to the next record
	}
}

int firstIBlock() {
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

char *disk = "fdimage";
main(int argc, char *argv[]) { 
	if (argc > 1) {
		disk = argv[1];
	}

	fd = open(disk, O_RDONLY);
	if (fd < 0){
		printf("open %s failed\n", disk);
		exit(1);
	}

	dir();
}