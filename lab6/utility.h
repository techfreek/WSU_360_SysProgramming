#ifndef utility_h
#define utility_h

#include "types.h"
#include "minode.h"
#include "mount.h"
#include "proc.h"

void get_block(int dev, int blk, char buf[]);
void put_block(int dev, int blk, char buf[]);
int tokenize(char *path, char names[64][128]);
char* dirname(const char* path);
char* basename(const char* path);
int getino(int dev, char *pathname);
int search(MINODE *mip, char *name);
int ialloc(int dev);
int balloc(int dev);
int idealloc(int dev, int ino);
int bdealloc(int dev, int bno);
int strEq(const char* str1, const char* str2);
int fs_init();
int quit();

#endif