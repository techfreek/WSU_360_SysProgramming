#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 256

typedef unsigned short u16;
typedef unsigned long  u32;

int	sock, //what I listen to
	newsock; //What I write to

char *cwd;

struct info {
    char type;  /* d, l, or - */
    char permissions[10]; /* wrx for user, group, other */
    u16 uid;
    u16 gid;
    u32 size;
    char* date;
};

void myls(char *pathname, int server);
void mycd(char *pathname, int server); //Should be implemented in the client/server file
void mymkdir(char *pathname, int server);
void myrmdir(char *pathname, int server);
void mycreat(char* pathname, int server);
void myrm(char* pathname, int server);
void myget(char* lpath, int server);
void myput(char* lpath, int server);
void getType(struct stat stats,  struct info *infom);
void getPerms(struct stat stats,  struct info *infom);
void transfer(char* lpath, char* dpath);

/*
Server communication protocol:
	//calling functions
	::func=(function name)[&path=(path)]
	ex ::func=ls&path=a

	//responses
	(plain text)

	//file header
	::filesize=(size)&name=(filename)
	ex ::filsize=9001&name=overninethousand.txt

	//file line
	::linesize=(size)&content=(content)
	ex ::linesize=1024&content=trollololololololololololol\n\0
*/