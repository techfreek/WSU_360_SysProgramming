#include "source.h"

extern Node* root;
extern Node* cwd;

extern char line[128];
extern char command[16];
extern char pathname[64];
extern char dirname[64];
extern char basename[64];

Node* followPath(char[]);
Node* getLastChild(Node*);
char* getNextDir(char[]);
char* getDirName(char[]);
char* getNextDir(char[]);
char* getBaseName(char[]);
Node* search(Node*,char*, char);
char* chopNChars(char[], int);

int mkdir(char pathname[]) {
	//parse path
		//if relative, follow path while checking if dir exists and is actually a dir
		//if absolute, scan path from root	
	//If the path is valid, check to make sure there is not another file with the same name
		//if so, then we create a new node
			//set child and sibling pointers to NULL,
			//if sibling
				//parentptr siblings parent(all children point at parent, not at siblings)
			//if child
				//set parentptr to parent
	printf("mkdir\n");
	char* temp = "";
	
	char basename[64] = "",
		 dirname[64] = "";
	Node *parentDir,
		 *newDir,
		 *oldestSibling;

	if(strlen(pathname) == 0) {
		printf("missing pathname/filename\n");
		return 0;
	} else {
		temp = getBaseName(pathname);
		if(temp == NULL) {
			strcpy(basename, pathname);
		} else {
			strcpy(basename, temp);
		}

		temp = getDirName(pathname);
		printf("temp: %s\n", temp);
		if(temp != NULL) strcpy(dirname, temp);
		if(strlen(dirname) > 0) {
			parentDir = followPath(dirname);
		} else {
			parentDir = cwd;
		}
		printf("Parent dir assigned as %s\n", parentDir->name);
		if(parentDir != NULL) {
			oldestSibling = getLastChild(parentDir);
			newDir = (Node *)malloc(sizeof(Node));
			newDir->type = 'D';
			strcpy(newDir->name, basename);
			newDir->siblingPtr = newDir->childPtr = NULL;
			newDir->parentPtr = parentDir;
			parentDir->childPtr = newDir;
			oldestSibling->siblingPtr = newDir; //Sets sibling pointer so it can be accessed again
			return 0;
		} else {
			return 0;
		}
	}
}

int rmdir(char pathname[]) {
	printf("rmdir coming soon\n");
	return 0;
	//parse path
		//if relative, follow path while checking if dir exists and is actually a dir
		//if absolute, scan path from root	
	//If the path is valid, check if basename exists and is a dir
		//if so, check if there are any child points
			//if no, we can delete
				//go to parent, and if it is the childptr, set sibling pointer to basename's sibling
				//delete node
			//if yes, then print file can't be deleted
}

int cd(char pathname[]) {
	printf("cd coming soon\n");
	return 0;
	//parse path
		//if NULL, set to root
		//if relative, follow path while checking if dir exists and is actually a dir
		//if absolute, scan path from root	
	//if valid and a dir
		//set cwd to that dir
}

int ls(char pathname[]) {
	Node* current;
	printf("cwd: %s\n", cwd->name);
	if(strlen(pathname) == 0) { //No path provided
		if(cwd->childPtr != NULL) {
			current = cwd->childPtr;
			do {
				printf("%s\n", current->name);
				current = current->siblingPtr;
			} while(current != NULL);
		}
	} else {
		printf("ls for other dirs coming soon\n");
	}
	//if NULL, print pwd
	//if not NULL,
		//verify path is good
	//if good path/NULL path
		//go to child ptr
		//print off filename
		//go to sibling
		//print off name
		//repeat
	return 0;
}

int pwd(char pathname[]) {
	printf("pwd coming soon\n");
	return 0;
	//ignore pathname[]
	//call rpwd with cwd node

}

int rpwd(Node *dir) {
	printf("rpwd coming soon\n");
	return 0;
	//if dir->parent == dir->child or dir->name == "/" (root)
		//print dir->name
		//return
	//else
		//call rpwd with parent of dir
		//print dir->name
}

int creat(char pathname[]) {
	printf("creat coming soon\n");
	return 0;
	//check if dirname is valid
		//if valid
			//check if there is another node with the same name as the basename
			//if not
				//create new node
				//set type to be 'F'
				//set name to basename
				//parent/child/sibling = NULL
				//last sibling of directory has sibling ptr pointed at file


}

int rm(char pathname[]) {
	printf("rm coming soon\n");
	return 0;
	//if valid path
		//check if file
		//set previous siblings sibling pointer to basename's sibling ptr
		//delete node

}

int save(char filename[]) {
	printf("save coming soon\n");
	return 0;
	//file name is what we are saving to
	//open file
	//call rsave with root
	//close file

}

int rsave(Node *file, FILE *fp) {
	printf("rsave coming soon\n");
	return 0;
	//recursive portion of save
	//output type then path to file
	//if file->sibling != NULL
		//call rsave on file->sibling
	//if file->child != NULL
		//call rsave on file->child
	//return
}


int reload(char filename[]) {
	printf("reload coming soon\n");
	return 0;
	//open file
	//read line
	//if type == "D" 
		//mkdir with path
	//if type == "F"
		//creat with path
	//close file
}

int quit(char filename[]) {
	printf("quit coming soon\n");
	return 0;
	//no filename passed, used as placeholder so it has the same signiture
	save("bahm_fs.txt");
}

Node* search(Node* startingPtr,char *basename, char type) {
	Node *current = startingPtr;
	printf("search coming soon\n");
	return 0;
	//Go to parent, return to first child. Makes sure we have the first sibling
	//Node* current = (startingPtr->parentPtr)->childPtr;
	while(current->siblingPtr != NULL && strcmp(current->name, basename) != 0) {
		current = current->siblingPtr;
		if(strcmp(current->name, basename) == 0 && type == current->type) {
			return current;
		}
	}

	//we are out of the loop here, AKA we didn't find it
	return NULL;
}

char* getBaseName(char path[]) {
	char* lastSlash = strrchr(path, '/');
	if(lastSlash == NULL) { //there is not path besides the file name
		printf("path didn't contain other dirs\n");
		return NULL;
	} else {
		return chopNChars(lastSlash, 1);
	}
	printf("lastSlash=%s\n", lastSlash);
	return lastSlash;
}

char* getDirName(char path[]) {
	 int path_length = strlen(path),
	 	 basename_length = 0;
	 char* end = strrchr(path, '/');
	 char* _dirname = malloc(sizeof(char) * 64);
	 if(end == NULL) {
	 	//if there is the dirname
	 	return NULL;
	 }
	 basename_length = strlen(end);
	 printf("path length: %d\n", (path_length - basename_length));
	 strncpy(_dirname, path, (path_length - basename_length));  //the minus one is to knock off the additional slash
	 printf("_dirname: %s\n", _dirname);
	 return  _dirname;
}

//If passed a/b/c/d, this will return a
char* getNextDir(char path[]) {
	char* _dir = strtok(path, '/');
	if(strlen(_dir) == 0) { //first character was a '/'
		_dir = strtok(NULL, '/'); //skip the the first actual dir name
	}
	return _dir;
}

//Scans until the last dir and returns that so we can easily add dirs to the end
Node* getLastChild(Node* dir) {
	Node* current = dir;
	if(current->childPtr != NULL) {
		current = current->childPtr;
		while(current->siblingPtr != NULL) {
			current = current->siblingPtr;
		}	
	}
	
	return current;
}

Node* followPath(char path[]) {
	char *dirs[64]; //Techincal limit of 32 for either, round up to be safe
	char backup[64];
	int i = 1,
		numDirs = 0,
		found = 0,
		valid = 1;
	Node* current;

	strcpy(backup, path);
	printf("path: %s\n", backup);
	//check if path is relative or absolute
	//if relative, starting point is cwd, otherwise root
	//Parse path into an array, and use that to look for the next dir?
	if(path[0] == '/') { //absolute path
		current = root->childPtr;
	} else {
		current = cwd->childPtr;
	}
	if(current != NULL) {
		printf("following path from: %s\n", current->name);

		//Initialize dirs array
		for(i = 0; i < 64; i++) {
			dirs[i] = malloc(64 * sizeof(char)); //allocate max of 64 chars in the array
		}
		printf("done allocting memory\n");
		if(strchr(backup, '/') == 0) { //only 1 dir deep
			strcpy(dirs[0], backup);
			numDirs = 1;
		} else {
			printf("first tok: %s\n", strtok(backup, '/'));
			strcpy(dirs[0], strtok(path, '/'));
			numDirs++;
			i = 1;
			printf("first strtok done\n");
			while(dirs[i-1] != NULL) {
				strcpy(dirs[i], strtok(NULL, '/'));
				//dirs[i] = strtok(NULL, '/');
				numDirs++;
				i++;
			} //populates array with the dir names
		}

		printf("done parsing path\n");

		//scans while we have not found it, and while the path exists
		i = 0;
		printf("starting to traverse path\n");
		while(valid && i < numDirs) {
			current = search(current, dirs[i], 'D'); //Look for dir
			if(current == NULL) {
				valid = 0;
				printf("Directory does not exist\n");
				return NULL;
			}
			i++;
		}
		printf("path traversed\n");
		return current;
	} else {
		return NULL;
	}
}

//Function to chop off characters from the front of a string. Mostly used to remove the leading slash
char* chopNChars(char str[], int chars) {
	int str_len = strlen(str);
	printf("chopping %d off %s which is %d chars long\n", chars, str, str_len);
	char* temp = malloc(sizeof(char) * 64);
	char newStr[64];

	if(chars > str_len) { //Don't even try if I am supposed to chop off more characters than I have
		return NULL;
	} else {
		//memmove(newStr, str + chars, str_len - chars + 1); //shifts chars over
		
		strncpy(temp, str + 1, str_len - 1);
		printf("made it past memmove\n");
		/*printf("made it past memmove\n");
		strcpy(temp, newStr);*/
		printf("newStr = %s\n", temp);
		return temp;
	}
}