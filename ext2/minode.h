#ifndef minode_h
#define minode_h

#include "utility.h"
#include "type.h"

//Need global MINODE *root variable

void initMINODE();
MINODE *iget(int devId, int ino);
int iput(MINODE *mip);
int findmyname(MINODE *parent, int myino, char *myname);
int findino(MINODE *mip, int myino, int parentino);
int isActive(int ino, int dev);
void printMINode(MINODE *mip);
void printAllMINodes();
int touch(MINODE *mip);
void closeAll();
MINODE *newMINODE();
MINODE *dupMINODE(MINODE *dupme);



#endif
