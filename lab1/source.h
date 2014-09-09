#ifndef SOURCE_H
#define SOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	char name[64];
	char type; //Either 'F' or 'D'
	struct Node *childPtr;
	struct Node *siblingPtr;
	struct Node *parentPtr;
} Node;

int mkdir(char[]);
int rmdir(char[]);
int cd(char[]);
int ls(char[]);
int inlinePWD();
int pwd(char[]);
int creat(char[]);
int rm(char[]);
int save(char[]);
int reload(char[]);

#endif