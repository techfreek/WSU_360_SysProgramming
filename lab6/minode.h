#ifndef minode_h
#define minode_h

#include "types.h"


void init();
MINODE *iget(int dev, int ino);
int iput(MINODE *mip);
int findmyname(MINODE *parent, int myino, char *myname);
int findino(MINODE *mip, int *myino, *parentino);

#endif