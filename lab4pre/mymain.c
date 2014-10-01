#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

typedef unsigned short u16;
typedef unsigned long  u32;

struct info {
	char type; 	/* d, l, or - */
	char permissions[10]; /* wrx for user, group, other */
	u32 uid;
	u32 size;
	char* date;
};

void getType(struct stat *stats,  struct info* infom);
void getPerms(struct stat *stats,  struct info* infom);

int main(int argc, char* argv[]) {
	if(argc > 1) {
		DIR *dp = opendir(argv[1]);      //opens a DIR (for R/W), and	
		if(dp) {
			struct dirent *ep = readdir(dp);   //returns ep pointing at the next entry of an opened DIR
			printf("Error: %s", strerror(errno));
			struct stat *stats;// = stat(ep->d_name);
			struct info *infom;
			printf("File_type     Permissions    Uid    Size     Creation Time\n");
			printf("------------   -------------   ----  -------  ----------------\n");
			while(ep != NULL) {
				stat(ep->d_name, stats);
				getType(stats, infom);
				getPerms(stats, infom);
				infom->uid = stats->st_uid;
				infom->size = stats->st_size;
				infom->date = (char*)ctime(&stats->st_ctime);
				printf("%c%s %lu %lu %s", infom->type, infom->permissions, infom->uid, infom->size, infom->date);
				ep = readdir(dp);
			}
		} else {
			printf("Could not open '%s'\n", argv[1]);
		}
	}
}

void getType(struct stat *stats,  struct info *infom) {
	if((stats->st_mode & 0100000) == 0100000) { //reg
		infom->type = '-';
	} else if((stats->st_mode & 0040000) == 0040000) { //dir
		infom->type = 'd';
	} else { //link
		infom->type = 'l';
	}
}

void  getPerms(struct stat *stats, struct info *infom) {
	/******* OWNER ****/
	if((stats->st_mode & (1 << 8)) ) { //==> Owner can r
		infom->permissions[0] = 'r';
	} else {
		infom->permissions[0] = '-';
	}

	if((stats->st_mode & (1 << 7)) ) { //==> Owner can w
		infom->permissions[1] = 'w';
	} else {
		infom->permissions[1] = '-';
	}

	if((stats->st_mode & (1 << 6)) ) { //==> Owner can x
		infom->permissions[2] = 'x';
	} else {
		infom->permissions[2] = '-';
	}

	/******* GROUP ****/
	if((stats->st_mode & (1 << 5)) ) { //==> can r
		infom->permissions[3] = 'r';
	} else {
		infom->permissions[3] = '-';
	}

	if((stats->st_mode & (1 << 4)) ) { //==> can w
		infom->permissions[4] = 'w';
	} else {
		infom->permissions[4] = '-';
	}

	if((stats->st_mode & (1 << 3)) ) { //==> can x
		infom->permissions[5] = 'x';
	} else {
		infom->permissions[5] = '-';
	}

	/******* OTHER ****/
	if((stats->st_mode & (1 << 2)) ) { //==> can r
		infom->permissions[6] = 'r';
	} else {
		infom->permissions[6] = '-';
	}

	if((stats->st_mode & (1 << 1)) ) { //==> can w
		infom->permissions[7] = 'w';
	} else {
		infom->permissions[7] = '-';
	}

	if((stats->st_mode & (1 << 0)) ) { //==> can x
		infom->permissions[8] = 'x';
	} else {
		infom->permissions[8] = '-';
	}
	infom->permissions[9] = '\0';

}