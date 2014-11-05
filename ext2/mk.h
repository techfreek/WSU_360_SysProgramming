#ifndef mk_h
#define mk_h

#include "type.h"
#include "utility.h"
#include "proc.h"
#include "fileops.h"
#include "minode.h"
#include "mount.h"

char *getLastDir(char buf[]);
int mycreat(char *path);
int make_dir(char *path);
int mymkdir(MINODE *parent, char *name);
int childExists(MINODE  *parent, char *childname);
int insertChild(MINODE *parent, MINODE *child, char *name);
int isIdeal(DIR *dp);
int calcIdeal(int len);

#endif