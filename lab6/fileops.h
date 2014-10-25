#ifndef fileops
#define fileops

#include "type.h"

int ls(char *pathname);
int list_file(MINODE* mip, char* name);
int list_dir(MINODE* mip);
int cd(char* pathname);
void pwd();
void rpwd();
STAT* stat();

#endif