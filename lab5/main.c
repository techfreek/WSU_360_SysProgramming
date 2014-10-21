#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include "source.h"

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

	if(!isEXT2(sp->s_magic)) {
		printf("Not an EXT2 drive!\n");
		exit(1);
	}

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


		
}