#ifndef SOURCE_H
#define SOURCE_H

typedef struct Node {
    char name[64];
    char type;
    Node *childPtr;
    Node *siblingPtr;
    Node *parentPtr;
} Node;

int mkdir(char *);
int rmdir(char *);
int cd(char*);
int ls(char *);
int pwd(char *);
int creat(char *);
int rm(char *);
int save(char *);
int reload(char *);

#endif