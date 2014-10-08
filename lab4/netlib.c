#include "netlib.h"
extern int sock, newsock;
extern char* cwd;

int myls(char *pathname, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}

	pid = fork();
	if(pid < 0) {
		fprintf(stderr, "Error forking: %s\n", strerror(errno));
	} else if(pid) {
		//Parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "LS ran into a problem: %s\n", strerror(errno));
			printf("LS ran into a problem: %s\n", strerror(errno));
		}
		endCommunication(clientFD);
	} else {
		struct stat stats;
		struct info *infom;

		if(pathname == NULL) {
			pathname = cwd;
		} else if(strlen(pathname) == 0) {
			strcpy(pathname, cwd);
		}
		
		DIR *dp = opendir(pathname);      //opens a DIR (for R/W), and  
		if(dp) {

			struct dirent *ep = readdir(dp);   //returns ep pointing at the next entry of an opened DIR
			
			while(ep != NULL) {
				lstat(ep->d_name, &stats);
				/*getType(stats, infom);
				getPerms(stats, infom);
				infom->uid = stats.st_uid;
				infom->gid = stats.st_gid;
				infom->size = stats.st_size;
				infom->date = (char*)ctime(&stats.st_ctime);
				infom->date[strlen(infom->date) - 1] = '\0'; //knocks off /n from the end of date*/
				//printf("%c%s %u %u %lu %s %s\n", infom->type, infom->permissions, infom->uid, infom->gid, infom->size,  infom->date, ep->d_name);
				//printf("%s ", ep->d_name);
				ep = readdir(dp);
				if(ep->d_name[0] != '.') {
					if(clientFD > 0) {
						write(outFD, ep->d_name, NETTRANS);
					} else {
						printf("%s \n", ep->d_name);
					}
				}
			}
		} else {
			printf("Could not open '%s'\n", pathname);
		}
		exit(0);
	} 

}

int mycd(char *pathname, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}

	pid = fork();
	if(pid < 0) {
		fprintf(stderr, "Error forking: %s\n", strerror(errno));
	} else if(pid) {
		//Parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "CD ran into a problem: %s\n", strerror(errno));
			printf("CD ran into a problem: %s\n", strerror(errno));
		}
	} else {
		chdir(pathname);
		exit(0);
	}
}

int mymkdir(char *pathname, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}
	
	if(pid < 0) {
		fprintf(stderr, "Error forking %s\n", strerror(errno));
	} else if(pid) {
		//parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "<function> ran into a problem: %s\n", strerror(errno));
			printf("<function> ran into a problem: %s\n", strerror(errno));
		}
	} else {
		//Put function here
		status = mkdir(pathname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		exit(0); //so child doesn't continue back to main loop
	}
}

int myrmdir(char *pathname, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}
	
	if(pid < 0) {
		fprintf(stderr, "Error forking %s\n", strerror(errno));
	} else if(pid) {
		//parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "rmdir ran into a problem: %s\n", strerror(errno));
			printf("rmdir ran into a problem: %s\n", strerror(errno));
		}
	} else {
		//Put function here
		status = rmdir(pathname);
		exit(0); //so child doesn't continue back to main loop
	}
}

int mycreat(char* pathname, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}
	
	if(pid < 0) {
		fprintf(stderr, "Error forking %s\n", strerror(errno));
	} else if(pid) {
		//parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "creat ran into a problem: %s\n", strerror(errno));
			printf("creat ran into a problem: %s\n", strerror(errno));
		}
	} else {
		//Put function here
		status = open(pathname, O_CREAT, 00755);
		exit(0); //so child doesn't continue back to main loop
	} 
}

int myrm(char* pathname, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}
	
	if(pid < 0) {
		fprintf(stderr, "Error forking %s\n", strerror(errno));
	} else if(pid) {
		//parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "rm ran into a problem: %s\n", strerror(errno));
			printf("rm ran into a problem: %s\n", strerror(errno));
		}
	} else {
		//Put function here
		status = unlink(pathname);
		exit(0); //so child doesn't continue back to main loop
	} 
}

int myget(char* lpath, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}
	
	if(pid < 0) {
		fprintf(stderr, "Error forking %s\n", strerror(errno));
	} else if(pid) {
		//parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "get ran into a problem: %s\n", strerror(errno));
			printf("get ran into a problem: %s\n", strerror(errno));
		}
	} else {
		//Put function here
		exit(0); //so child doesn't continue back to main loop
	}
}

int myput(char* lpath, int clientFD) {
	int pid = 0, 
		status = 0,
		r = 0,
		outFD = STDIN_FILENO;
	
	if(clientFD > 0) {
		outFD = clientFD;
	}
	
	if(pid < 0) {
		fprintf(stderr, "Error forking %s\n", strerror(errno));
	} else if(pid) {
		//parent
		pid = wait(&r);
		if(r > 0) {
			fprintf(stderr, "put ran into a problem: %s\n", strerror(errno));
			printf("put ran into a problem: %s\n", strerror(errno));
		}
	} else {
		//Put function here
		exit(0); //so child doesn't continue back to main loop
	}
}

int functionLookup(char* cmd) {
	char *commands[] = {"ls", "cd", "mkdir", "rmdir", "creat", "rm", "get", "put", 0};
	int i = 0;
	while(commands[i] != NULL) {
		if(strcmp(cmd, commands[i]) == 0) {
			return i;
		}
		i++;
	}
	return -1;
}


/*
	Looks up the correct function, calls it and passes along the int signifying if it is the clientFD or client
*/
int callFunction(int funcID, char* pathname, int clientFD) {
	int r = -1;
	if(funcID >= 0) {
		int (*fptr[ ])(char *, int) = {(int (*)())myls, mycd, mymkdir, myrmdir, mycreat, myrm, myget, myput};	
		r = fptr[funcID](pathname, clientFD);
	}
	return r;
}


void transfer(char* lpath, char* dpath) {
	/* handles the actual put/get process */
}

void getType(struct stat stats,  struct info *infom) {
	if((stats.st_mode & 0100000) == 0100000) { //reg
		infom->type = '-';
	} else if((stats.st_mode & 0040000) == 0040000) { //dir
		infom->type = 'd';
	} else { //link
		infom->type = 'l';
	}
}

void  getPerms(struct stat stats, struct info *infom) {
	/******* OWNER ****/
	if((stats.st_mode & (1 << 8)) ) { //==> Owner can r
		infom->permissions[0] = 'r';
	} else {
		infom->permissions[0] = '-';
	}

	if((stats.st_mode & (1 << 7)) ) { //==> Owner can w
		infom->permissions[1] = 'w';
	} else {
		infom->permissions[1] = '-';
	}

	if((stats.st_mode & (1 << 6)) ) { //==> Owner can x
		infom->permissions[2] = 'x';
	} else {
		infom->permissions[2] = '-';
	}

	/******* GROUP ****/
	if((stats.st_mode & (1 << 5)) ) { //==> can r
		infom->permissions[3] = 'r';
	} else {
		infom->permissions[3] = '-';
	}

	if((stats.st_mode & (1 << 4)) ) { //==> can w
		infom->permissions[4] = 'w';
	} else {
		infom->permissions[4] = '-';
	}

	if((stats.st_mode & (1 << 3)) ) { //==> can x
		infom->permissions[5] = 'x';
	} else {
		infom->permissions[5] = '-';
	}

	/******* OTHER ****/
	if((stats.st_mode & (1 << 2)) ) { //==> can r
		infom->permissions[6] = 'r';
	} else {
		infom->permissions[6] = '-';
	}

	if((stats.st_mode & (1 << 1)) ) { //==> can w
		infom->permissions[7] = 'w';
	} else {
		infom->permissions[7] = '-';
	}

	if((stats.st_mode & (1 << 0)) ) { //==> can x
		infom->permissions[8] = 'x';
	} else {
		infom->permissions[8] = '-';
	}
	infom->permissions[9] = '\0';

}

/* Protocol Accessors */

char* getFunction(const char line[]) {
	char* temp = strdup(line);
	char* function = strstr(temp, "func=");

	if(function) {
		function += 5;
	
		int endIndex = 0;

		endIndex = strcspn(function, "&");
		if(endIndex != strlen(function)) { //there is a second header after this
			memset(function + endIndex, 0, strlen(function) - endIndex);
		}

		return function;
	} else {
		return NULL;
	}
}

char* getPath(const char line[]) {
	char* temp = strdup(line);
	char* path = strstr(temp, "path=");

	if(path) {
		path += 5;
		int endIndex = 0;

		endIndex = strcspn(path, "&");
		if(endIndex != strlen(path)) { //there is a second header after this
			memset(path + endIndex, 0, strlen(path) - endIndex);
		}
		return path;
	} else {
		return NULL;
	}
}

int getFilesize(const char line[]) {
	int filesize = 0;
	char* temp = strdup(line);
	char* filesize_str = strstr(temp, "filesize=");
	int endIndex = 0;

	if(filesize_str) {
		filesize += 9;
		endIndex = strcspn(filesize_str, "&");
		if(endIndex != strlen(filesize_str)) { //there is a second header after this
			//strncpy(filesize_str, filesize_str, endIndex);  //truancate before next header
			memset(filesize_str + endIndex, 0,  strlen(filesize_str) - endIndex);
		}
		return atoi(filesize_str);
	} else {
		return NULL;
	}

}

char* getFilename(const char line[]) {
	/*char* temp = strdup(line);
	char* function = strstr(temp, "func=");

	if(function) {
		function += 5;
	
		int endIndex = 0;

		endIndex = strcspn(function, "&");
		if(endIndex != strlen(function)) { //there is a second header after this
			memset(function + endIndex, 0, strlen(function) - endIndex);
		}

		return function;
	} else {
		return NULL;
	}*/

	char* temp = strdup(line);
	char* filename = strstr(temp, "name=");
	int endIndex = 0;

	if(filename) {
		filename += 5;
		endIndex = strcspn(filename, "&");
		if(endIndex != strlen(filename)) { //there is a second header after this
			//strncpy(filename, filename, endIndex);  //truancate before next header
			memset(filename + endIndex, 0, strlen(filename) - endIndex);
		}
		return filename;
	} else {
		return NULL;
	}
}

int getLinesize(const char line[]) {
	char* temp = strdup(line);
	char* linesize_str = strstr(temp, "linesize=");
	int endIndex = 0;

	if(linesize_str) {
		linesize_str += 9;
		endIndex = strcspn(linesize_str, "&");
		if(endIndex != strlen(linesize_str)) { //there is a second header after this
			//strncpy(linesize_str, linesize_str, endIndex); //truancate before next header
			memset(linesize_str + endIndex, 0, strlen(linesize_str) - endIndex);
		}
		return atoi(linesize_str);
	} else {
		return -1;
	}

}

char* getContent(const char line[]) {
	char* temp = strdup(line);
	char* content = strstr(temp, "content=");
	int endIndex = 0;

	if(content) {
		content += 8;
		endIndex = strcspn(content, "&");
		if(endIndex != strlen(content)) { //there is a second header after this
			//strncpy(content, content, endIndex);
			memset(content + endIndex, 0, strlen(content) - endIndex);
		}
		return content;
	} else {
		return NULL;
	}
}

void endCommunication(int clientFD) {
	if(clientFD > 0) {
		write(clientFD, "::DONE", NETTRANS);		
		printf("Ending client communication\n");
	}
}