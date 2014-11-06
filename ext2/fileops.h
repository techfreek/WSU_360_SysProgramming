#ifndef fileops
#define fileops

#include "type.h"

int ls(char *pathname);
int list_file(INODE *ip, char* name);
int list_dir(MINODE* mip);
int getDIRFileName(DIR* dp, char buf[]) ;
int cd(char* pathname);
void pwd();
MINODE* rpwd(MINODE *mip);
//STAT* stat();

#endif