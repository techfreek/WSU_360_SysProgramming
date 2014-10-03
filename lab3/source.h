#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int executeCMDs(char* cmds[], int numCommands);
int executeCMD(char* args[], char* args2[], int numArgs, int numArgs2);