/********* super.c code ***************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

#define BLKSIZE 1024

int gd();

char buf[BLKSIZE];
int fd;
char *disk = "fdimage";

main(int argc, char *argv[]) { 
	if (argc > 1) {
		disk = argv[1];
	}

	fd = open(disk, O_RDONLY);
	if (fd < 0){
		printf("open failed\n");
		exit(1);
	}

	gd();
}


int get_block(int fd, int blk, char buf[]) {
	lseek(fd, (long)blk*BLKSIZE, 0);
	read(fd, buf, BLKSIZE);
}

int gd() {
	// read group block
	get_block(fd, 2, buf);  
	gp = (GD *)buf;

	printf("bmap block #: %d\n", gp->bg_block_bitmap);
	printf("imap block #: %d\n", gp->bg_inode_bitmap);

	printf("Start inode block #: %d\n", gp->bg_inode_table);

	printf("free blocks: %d\n", gp->bg_free_blocks_count);
	printf("free inodes: %d\n", gp->bg_free_inodes_count);

	printf("# used dirs: %d\n", gp->bg_used_dirs_count);
	return 0;
}