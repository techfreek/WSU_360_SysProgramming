#ifndef minode_h
#define minode_h

#include "types.h"

//Need global MINODE *root variable

void initMINODE();
MINODE *iget(int devId, int ino);
int iput(MINODE *mip);
int findmyname(MINODE *parent, int myino, char *myname);
int findino(MINODE *mip, int *myino, *parentino);
void printMINode(MINODE *mip);

#endif
