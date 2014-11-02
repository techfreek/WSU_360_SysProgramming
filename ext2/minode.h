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
void printMINode(MINODE *mip);
void printAllMINodes();
void closeAll();
MINODE *newMINODE();
MINODE *dupMINODE(MINODE *dupme);

#endif
