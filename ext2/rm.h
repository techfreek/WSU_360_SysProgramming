#ifndef rm_h
#define rm_h

#include "proc.h"
#include "type.h"
#include "utility.h"
#include "fileops.h"
#include "minode.h"
#include "mk.h"


int myunlink(char *path);
int myrmdir(char *path);
char* inodeExists(int ino, char buf[], char **pp);
int removechild(int ino, int devId);

#endif