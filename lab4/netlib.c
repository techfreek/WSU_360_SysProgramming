#include "netlib.h"
extern int sock, newsock;
extern char* cwd;

void myls(char *pathname, int server) {
	int pid = 0, 
		status = 0,
		r = 0;
	
	if(server) {
		close(1);
		dup2(newsock, 1);
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
		struct stat stats;
		struct info *infom;

		if(strlen(pathname) == 0) {
			strcpy(pathname, cwd);
		}
		
		DIR *dp = opendir(pathname);      //opens a DIR (for R/W), and  
		if(dp) {

			struct dirent *ep = readdir(dp);   //returns ep pointing at the next entry of an opened DIR
			
			while(ep != NULL) {
				lstat(ep->d_name, &stats);
				getType(stats, infom);
				getPerms(stats, infom);
				infom->uid = stats.st_uid;
				infom->gid = stats.st_gid;
				infom->size = stats.st_size;
				infom->date = (char*)ctime(&stats.st_ctime);
				infom->date[strlen(infom->date) - 1] = '\0'; //knocks off /n from the end of date
				printf("%c%s %u %u %lu %s %s\n", infom->type, infom->permissions, infom->uid, infom->gid, infom->size,  infom->date, ep->d_name);
				ep = readdir(dp);
			}
		} else {
			printf("Could not open '%s'\n", pathname);
		}
		
		exit(0);
	}

}

void mycd(char *pathname, int server) {
	int pid = 0, 
		status = 0,
		r = 0;

	if(server) {
		close(1);
		dup2(newsock, 1);
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

void mymkdir(char *pathname, int server) {
	int pid = 0, 
		status = 0,
		r = 0;

	if(server) {
		close(1);
		dup2(newsock, 1);
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

void myrmdir(char *pathname, int server) {
	int pid = 0, 
		status = 0,
		r = 0;

	if(server) {
		close(1);
		dup2(newsock, 1);
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

void mycreat(char* pathname, int server) {
	int pid = 0, 
		status = 0,
		r = 0;

	if(server) {
		close(1);
		dup2(newsock, 1);
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

void myrm(char* pathname, int server) {
	int pid = 0, 
		status = 0,
		r = 0;

	if(server) {
		close(1);
		dup2(newsock, 1);
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

void myget(char* lpath, int server) {
	int pid = 0, 
		status = 0,
		r = 0;

	if(server) {
		close(1);
		dup2(newsock, 1);
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

void myput(char* lpath, int server) {
	int pid = 0, 
		status = 0,
		r = 0;

	if(server) {
		close(1);
		dup2(newsock, 1);
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
	char function = strstr(temp, "func=");
	int endIndex = 0;

	if(function) {
		endIndex = strcspn(function, "&");
		if(endIndex != strlen(function)) { //there is a second header after this
			strncpy(function, function, endIndex); //truancate before next header
		}
		return function;
	} else {
		return NULL;
	}
}

char* getPath(const char line[]) {
	char* temp = strdup(line);
	char path = strstr(temp, "path=");
	int endIndex = 0;

	if(path) {
		endIndex = strcspn(path, "&");
		if(endIndex != strlen(path)) { //there is a second header after this
			strncpy(path, path, endIndex); //truancate before next header
		}
		return path;
	} else {
		return NULL;
	}
}

int getFilesize(const char line[]) {
	int filesize = 0;
	char* temp = strdup(line);
	char filesize_str = strstr(temp, "func=");
	int endIndex = 0;

	if(filesize_str) {
		endIndex = strcspn(filesize_str, "&");
		if(endIndex != strlen(filesize_str)) { //there is a second header after this
			strncpy(filesize_str, filesize_str, endIndex);  //truancate before next header
		}
		return atoi(filesize_str);
	} else {
		return NULL;
	}

}

char* getFilename(const char line[]) {
	char* temp = strdup(line);
	char filename = strstr(temp, "func=");
	int endIndex = 0;

	if(filename) {
		endIndex = strcspn(filename, "&");
		if(endIndex != strlen(filename)) { //there is a second header after this
			strncpy(filename, filename, endIndex);  //truancate before next header
		}
		return filename;
	} else {
		return NULL;
	}
}

int getLinesize(const char line[]) {
	char* temp = strdup(line);
	char linesize_str = strstr(temp, "linesize=");
	int endIndex = 0;

	if(linesize_str) {
		endIndex = strcspn(linesize_str, "&");
		if(endIndex != strlen(linesize_str)) { //there is a second header after this
			strncpy(linesize_str, linesize_str, endIndex); //truancate before next header
		}
		return atoi(linesize_str);
	} else {
		return -1;
	}

}

char* getContent(const char line[]) {
	char* temp = strdup(line);
	char content = strstr(temp, "content=");
	int endIndex = 0;

	if(content) {
		endIndex = strcspn(content, "&");
		if(endIndex != strlen(content)) { //there is a second header after this
			strncpy(content, content, endIndex);
		}
		return content;
	} else {
		return NULL;
	}
}