#ifndef utility_h
#define utility_h

#include "types.h"
#include "minode.h"
#include "mount.h"
#include "proc.h"

void get_block(int devId, int blk, char buf[]);
void put_block(int devId, int blk, char buf[]);
int tokenize(char *path, char names[64][128]);
char* dirname(const char* path);
char* basename(const char* path);
int getino(int devId, char *pathname);
INODE* get_inode(int devId, int ino);
int search(MINODE *mip, char *name);
int ialloc(int devId);
int balloc(int devId);
int idealloc(int devId, int ino);
int bdealloc(int devId, int bno);
int strEq(const char* str1, const char* str2);
int updateFreeInodes(int devId, int delta);
int updateFreeBlocks(int devId, int delta);
int fs_init();
int quit();
int isEXT2(u32 magic);
void printInode(INODE* ip);

#endif