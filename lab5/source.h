#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs
typedef unsigned int   u32;
typedef struct DIR* SHUTUP; //tell the compiler to shut up

#define BLKSIZE 1024

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

int isEXT2(u32 magic);
int get_block(int fd, int blk, char buf[]);
int get_inode(int fd, int ino, int startInoTable, INODE* node);
void printSuper(int fd);
int firstIBlock(int fd);
int search(int fd, int inoStart, char names[64][128], int dirsRemaining, int ino);
int getIno(int fd, int inoStart, char *path);
int tokenize(char *path, char names[64][128]);
int isStrEq(const char* str1, const char* str2);
void printBlocks(int fd, INODE* file);