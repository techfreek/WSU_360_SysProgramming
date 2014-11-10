#ifndef link_h
#define link_h

#include "type.h"
#include "utility.h"
#include "proc.h"
#include "fileops.h"
#include "minode.h"
#include "mount.h"
#include "mk.h"
#include "rm.h"

int link(char *old_file, char *new_file);
int symlink(char *old_name, char *new_file);
int unlink(char *pathname);

#endif