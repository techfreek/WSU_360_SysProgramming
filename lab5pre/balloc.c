#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

#define BLKSIZE 1024

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

int get_block(int fd, int blk, char buf[]);
int put_block(int fd, int blk, char buf[]);
int tst_bit(char *buf, int bit);
int set_bit(char *buf, int bit);
int clr_bit(char *buf, int bit);
int clr_bit(char *buf, int bit);
int updateFreeBlocks(int dev, int delta);
int balloc(int dev);

/********** globals *************/
int fd;
int imap, bmap;  // IMAP and BMAP block number
int ninodes, blocks, nfreeInodes, nfreeBlocks;

char *disk = "fdimage";

main(int argc, char *argv[]) {
	int i, bno;
	char buf[BLKSIZE];

	if (argc > 1) {
		disk = argv[1];
	}

	fd = open(disk, O_RDWR);
	if (fd < 0) {
		printf("open %s failed\n", disk);
		exit(1);
	}

	// read SUPER block
	get_block(fd, 1, buf);
	sp = (SUPER *)buf;

	ninodes = sp->s_inodes_count;
	blocks = sp->s_blocks_count;
	nfreeInodes = sp->s_free_inodes_count;
	nfreeBlocks = sp->s_free_blocks_count;
	printf("ninodes=%d nblocks=%d nfreeInodes=%d nfreeBlocks=%d\n", 
		ninodes, blocks, nfreeInodes, nfreeBlocks);

	// read Group Descriptor 0
	get_block(fd, 2, buf);
	gp = (GD *)buf;

	bmap = gp->bg_block_bitmap;
	printf("bmap = %d\n", bmap);
	getchar();

	for (i=0; i < 5; i++){  
		bno = balloc(fd);
		printf("allocated bno = %d\n", bno);
	}
}

int get_block(int fd, int blk, char buf[]) {
	lseek(fd, (long)blk*BLKSIZE, 0);
	read(fd, buf, BLKSIZE);
}

int put_block(int fd, int blk, char buf[]) {
	lseek(fd, (long)blk*BLKSIZE, 0);
	write(fd, buf, BLKSIZE);
}

int tst_bit(char *buf, int bit) {
	int i, j;
	i = bit/8; j=bit%8;
	if (buf[i] & (1 << j)) {
		return 1;
	}

	return 0;
}

int set_bit(char *buf, int bit) {
	int i, j;
	i = bit/8; j=bit%8;
	buf[i] |= (1 << j);
}

int clr_bit(char *buf, int bit) {
	int i, j;
	i = bit/8; j=bit%8;
	buf[i] &= ~(1 << j);
}

//Pass 1 to increase by 1, -1 to decrease by 1, etc
int updateFreeBlocks(int dev, int delta) {
	char buf[BLKSIZE];

	// update free block count in SUPER and GD
	get_block(dev, 1, buf);
	sp = (SUPER *)buf;
	sp->s_free_blocks_count + delta;
	put_block(dev, 1, buf);

	get_block(dev, 2, buf);
	gp = (GD *)buf;
	gp->bg_free_blocks_count + delta;
	put_block(dev, 2, buf);
}

int balloc(int dev) {
	int  i;
	char buf[BLKSIZE];

	// read block_bitmap block
	get_block(dev, bmap, buf);

	for (i=0; i < blocks; i++){
		if (tst_bit(buf, i)==0){
			 set_bit(buf,i);
			 updateFreeBlocks(dev, 1);

			 put_block(dev, bmap, buf);

			 return i+1;
		}
	}
	
	printf("balloc(): no more free inodes\n");
	return 0;
}

//which deallocates an inode number, ino
int idealloc(int dev, int ino) {
	char buf[1024];
	get_block(dev, imap, buf);
	clr_bit(buf, ino);
	put_block(dev, imap, buf);
}

//which deallocates an block number, bno
int bdealloc(int dev, int bno) {
	char buf[1024];
	get_block(dev, bmap, buf);
	clr_bit(buf, bno);
	put_block(dev, bmap, buf);
}
