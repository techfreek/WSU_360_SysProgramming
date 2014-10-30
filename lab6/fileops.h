#ifndef fileops
#define fileops

#include "type.h"

int ls(char *pathname);
int list_file(MINODE* mip, char* name);
int list_dir(MINODE* mip);
int getDIRFileName(DIR* dp, char buf[]) ;
int cd(char* pathname);
void pwd();
MINODE* rpwd(MINODE *mip);
//STAT* stat();

#endif