#ifndef rm_h
#define rm_h

#include "proc.h"
#include "type.h"
#include "utility.h"
#include "fileops.h"
#include "minode.h"
#include "mk.h"


int myrmdir(char *path);
char* inodeExists(MINODE *parent, int ino, char buf[], char **pp);
int removechild(MINODE *parent, int ino, int devId);
int isEmptyDir(INODE *ip, int devId);
int clearBlocks(INODE *ip, int devId);

#endif