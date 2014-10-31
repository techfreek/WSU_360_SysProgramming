#ifndef proc_h
#define proc_h

#include "type.h"

PROC* newProc(int uid);
int removeProc(int uid);
void printAllProcs();
void killAllProcs();

#endif
