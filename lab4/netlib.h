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
#define NETTRANS 2048
#define FILELINE 1024
#define null NULL

typedef unsigned short u16;
typedef unsigned long  u32;

int	sock, //what I listen to
	newsock; //What I write to

char *cwd;
char *rootcwd;

struct info {
    char type;  /* d, l, or - */
    char permissions[10]; /* wrx for user, group, other */
    u16 uid;
    u16 gid;
    u32 size;
    char* date;
};

int myls(char *pathname, int clientFD);
int mycd(char *pathname, int clientFD); //Should be implemented in the client/server file
int mymkdir(char *pathname, int clientFD);
int myrmdir(char *pathname, int clientFD);
int mycreat(char* pathname, int clientFD);
int myrm(char* pathname, int clientFD);
int myget(char* lpath, int clientFD);
int myput(char* lpath, int clientFD);
int mypwd(char* path, int clientFD);
void getType(struct stat stats,  struct info *infom);
void getPerms(struct stat stats,  struct info *infom);
void transfer(char* filename, int fd);


int functionLookup(char* cmd);
int callFunction(int funcID, char* pathname, int clientFD);

/* Protocol Accessors, caller must clean up memory */
char* getFunction(const char line[]);
char* getPath(const char line[]);
int getFilesize(const char line[]);
char* getFilename(const char line[]);
int getLinesize(const char line[]);
char* getContent(const char line[]);

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

	//end of communication
	::DONE
*/