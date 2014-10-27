#ifndef minode_h
#define minode_h

#include "types.h"

MINODE *iget(int devId, int ino);
int iput(MINODE *mip);
int findmyname(MINODE *parent, int myino, char *myname);
int findino(MINODE *mip, int *myino, *parentino);
void printMINode(MINODE *mip);

#endif