#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include "source.h"

/********** globals *************/
int fd;
int imap, bmap;  // IMAP and BMAP block number
int ninodes, blocks, nfreeInodes, nfreeBlocks;

char *diskname = "fdimage";

main(int argc, char *argv[]) {
	int i, bno;
	char buf[BLKSIZE];
	char path[128];
	DEV* disk = (DEV *)malloc(sizeof(DEV));

	if(argc == 1) {
		printf("Please provide a path\n");
		exit(1);
	} else if (argc == 2) {
		strncpy(path, argv[1], 128);
	} else {
		diskname = argv[1];
		strncpy(path, argv[2], 128);
		printf("Path: %s\n", path);
	}

	disk->fd = open(diskname, O_RDWR);
	if (disk->fd < 0) {
		printf("open %s failed\n", disk);
		exit(1);
	}

	// read SUPER block
	get_block(disk->fd, 1, buf);
	sp = (SUPER *)buf;

	if(!isEXT2(sp->s_magic)) {
		printf("Not an EXT2 drive!\n");
		exit(1);
	}

	disk->ninodes = sp->s_inodes_count;
	disk->blocks = sp->s_blocks_count;
	disk->nfreeBlocks = sp->s_free_inodes_count;
	disk->nfreeInodes = sp->s_free_blocks_count;
	printf("ninodes=%d nblocks=%d nfreeInodes=%d nfreeBlocks=%d\n", 
		disk->ninodes, disk->blocks, disk->nfreeInodes, disk->nfreeBlocks);

	// read Group Descriptor 0
	get_block(disk->fd, 2, buf);
	gp = (GD *)buf;

	disk->bmap = gp->bg_block_bitmap;
	disk->imap = gp->bg_block_bitmap;
	disk->nfreeInodes = nfreeInodes;
	disk->bg_inode_table = gp->bg_inode_table;

	printf("bmap = %d\n", disk->bmap);
	getchar();

	getIno(disk, path);
		
}