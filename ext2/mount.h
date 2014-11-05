#ifndef mount_h
#define mount_h

#include "type.h"

int mountDisk(int fd, MINODE *mounted_inode, char name[64], char mount_name[64]);
int unMountDisk(int disk);
int unmountAll();
int getFD(int disk);
MINODE* getMountedInode(int disk);
char* getName(int disk);
char* getMountName(int disk);
int getNBlocks(int disk);
int getNInodes(int disk);
int getBMap(int disk);
int getDevID(int fd);
int getIMap(int disk);
int getIBlk(int disk);
void printAllMounts();
void printMountPtr(MOUNT* dp);
void printMount(int disk);
MOUNT* getMountPtr(int disk);

#endif
