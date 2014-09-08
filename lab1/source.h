#ifndef SOURCE_H
#define SOURCE_H

typedef struct Node {
    char name[64];
    char type; //Either 'F' or 'D'
    Node *childPtr;
    Node *siblingPtr;
    Node *parentPtr;
} Node;

extern Node* root;
extern Node* cwd;
char line[128];
char command[16];
char pathname[64];
char dirname[64];
char basename[64];

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