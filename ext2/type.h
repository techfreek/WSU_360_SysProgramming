#ifndef types_h
#define types_h

/* type.h file for CS360 FS */

#include <stdio.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>   // MAY NEED "ext2_fs.h"
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs
typedef struct DIR* SHUTUP; //tell the compiler to shut up
typedef unsigned int u32;

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

#define BLOCK_SIZE     1024
#define BLKSIZE 1024

// Block number of EXT2 FS on FD
#define SUPERBLOCK        1
#define GDBLOCK           2
#define ROOT_INODE        2

// Default dir and regulsr file modes
#define DIR_MODE    0040777 
#define FILE_MODE   0100644
#define SUPER_MAGIC  0xEF53
#define SUPER_USER        0

// Proc status
#define FREE              0
#define READY             1
#define RUNNING           2

// Table sizes
#define NMINODES        100
#define NOFT            100
#define NNAME			 64
#define NMOUNT           10
#define NPROC            10
#define NFD              10

// i_block indexs
#define SINGLEINDIRECT   13
#define DBLINDIRECT      14
#define TRIPLEINDIRECT   15

// Open File Table
typedef struct oft{
	int   mode;
	int   refCount;
	struct minode *inodeptr;
	int   offset;
}OFT;

// PROC structure
typedef struct proc{
	struct proc *next;
	int   uid;
	int   pid, gid;
	int   status;
	struct minode *cwd;
	int cwdDevId; //store the devId of the current directory
	OFT   *fd[NFD];
}PROC;

// In-memory inodes structure
typedef struct minode{		
	INODE INODE;               // disk inode
	int   dev, ino;
	int   refCount;
	int   dirty;
	int   mounted;
	struct mount *mountptr;
}MINODE;

// Mount Table structure
typedef struct mount{
	int    dev;
	int    nblocks,ninodes;
	int    bmap, imap, iblk;
	MINODE *mounted_inode;
	char   name[NNAME]; 
	char   mount_name[NNAME];
}MOUNT;

/*
	struct ext2_inode {
		//*************************************************************************
		u16  i_mode;          // same as st_mode in stat() syscall
		u16  i_uid;                     // ownerID
		u32  i_size;                    // file size in bytes
		u32  i_atime;                   // time fields  
		u32  i_ctime;
		u32  i_mtime;
		u32  i_dtime;
		u16  i_gid;                     // groupID
		u16  i_links_count;             // link count
		u32  i_blocks;                  // IGNORE
		u32  i_flags;                   // IGNORE
		u32  i_reserved1;               // IGNORE
		u32  i_block[15];               // See details below
		//**************************************************************************

		// IGNORE THE FOLLOWING FIELDS
		u32  i_version;
		u32  i_file_acl;
		u32  i_dir_acl;
		u32  i_faddr;
		u8   i_frag;
		u8   i_fsize;
		u16  i_pad1;
		u32  i_reserved2[2];
	}

*/

#endif
