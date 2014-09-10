#include "source.h"

extern Node* root;
extern Node* cwd;

extern char line[128];
extern char command[16];
extern char pathname[64];
extern char dirname[64];
extern char basename[64];

int make(char[], char);
int removefile(char[], char);
Node* followPath(char[]);
Node* getLastChild(Node*);
int rpwd(Node *, char *);
char* getNextDir(char[]);
char* getDirName(char[]);
char* getNextDir(char[]);
char* getBaseName(char[]);
Node* search(Node*,char*, char);
char* chopNChars(char[], int);

int mkdir(char pathname[]) {
	//Calls the generalized make function to make a Node of type 'D'
	return make(pathname, 'D');
}

int rmdir(char pathname[]) {
	//Calls generalized remove function to look for the specified 'D' and delete it if possible
	return removefile(pathname, 'D');
}

int cd(char pathname[]) {
	/*
		If the path is valid (we can follow it)
			change cwd to temp
		else
			print why
	*/

	Node* temp = followPath(pathname);
	if(temp != NULL) {
		if(temp->type == 'D') { //Double check temp is actually a directory
			cwd = temp;
		} else {
			printf("%s is not a directory\n", temp->name);
		}
	} else {
		printf("No such file or directory\n");
	}
	return 0;
}

int ls(char pathname[]) {
	/*
		If path exists, follow path
			if path is null
				set path to null, AKA do nothing
			else 
				set current as the destination dir
		else 
			set current as destination dir

		if exists, go to child of current
			print file name
			while siblingPtr exists, go to sibling and repeat
	*/

	Node* current;
	printf("cwd: %s\n", cwd->name);
	if(strlen(pathname) == 0) { //No path provided
		if(cwd->childPtr != NULL) {
			current = cwd->childPtr;
		} else {
			current = NULL;
		}
	} else {
		current = followPath(pathname);
	}
	if(current != NULL) {
		do {
			printf("%s\t", current->name);
			current = current->siblingPtr;
		} while(current != NULL);
	}
	printf("\n");

	return 0;
}

char* getPWD(Node* source) {
	//Allocates a string to have things appended to
	char *pwdPath = malloc(sizeof(char) * 128);
	strcpy(pwdPath, ""); //init it
	if(source == NULL) {
		source = cwd;
	} else if(source == root) {
		strcpy(pwdPath, "/"); //if we start from root, return a hardcoded '/'
		return pwdPath;
	}
	rpwd(source, pwdPath); //if not root, recursively call to print off all files
	return pwdPath;
}

int pwd(char pathname[]) {
	//ignore pathname[]
	//call rpwd with cwd node	
	rpwd(cwd, NULL); //Doesn't pass string, am not returning it
	printf("/\n"); //newline after the string
	return 0;
}

int rpwd(Node *dir, char *string) {
	/*
	if dir->parent == dir->child or dir->name == "/" (root)
		dont need to print because the name is just '/'
		return
	else
		call rpwd with parent of dir
		if string exists
			append dirname to string
		else
			print dir->name
		return
	*/
	if(dir->parentPtr == dir->siblingPtr) { //check if root node
		return 0;
	} else {
		rpwd(dir->parentPtr, string);
		
		if(string != NULL) {
			strcat(string, "/");
			strcat(string, dir->name);
		} else {
			printf("%s/", dir->name);
		}
		return 0;
	}
}

int creat(char pathname[]) {
	/*utilize generalized make function that can make files and dirs*/
	return make(pathname, 'F');
}

int rm(char pathname[]) {
	return removefile(pathname, 'F');
	//if valid path
		//check if file
		//set previous siblings sibling pointer to basename's sibling ptr
		//delete node

}

int save(char filename[]) {
	//file name is what we are saving to
		//if filename is not specified, go to default
	//open file
	//call rsave with root
	//close file
	FILE *output;
	if(strlen(filename) == 0) {
		printf("No file name supplied. Defaulting to \"bahm_fs\"\n");
		strcpy(filename, "bahm_fs");
	}

	output = fopen(filename, "w");
	rsave(root, output);

	fclose(output);

	return 0;
}

int rsave(Node *file, FILE *fp) {
	//recursive portion of save
	//output type then path to file, get path from getPWD
	//clear up wd string because that is dynamically allocated
	//if file->sibling != NULL
		//call rsave on file->sibling
	//if file->child != NULL
		//call rsave on file->child
	//return
	char* wd = getPWD(file);
	fprintf(fp, "%c %s\n", file->type, wd);
	free(wd);
	if(file->childPtr != NULL) {
		rsave(file->childPtr, fp);
	}
	if(file->siblingPtr != NULL && file->siblingPtr != file) { //Need to check if it's root trying to go to itself
		rsave(file->siblingPtr, fp);
	}

	return 0;
}


int reload(char filename[]) {
	//open file
	//read line
	//if type == "D" 
		//mkdir with path
	//if type == "F"
		//creat with path
	//close file
	FILE *filesys; 
	char line[100];
	char type = "";
	int parseStatus = 0;
	if(strlen(filename) == 0) {
		printf("No file name supplied. Defaulting to \"bahm_fs\"\n");
		strcpy(filename, "bahm_fs");
	}

	filesys = fopen(filename, "r");
	if(filesys == NULL) {
		puts("Error opening file");
	} else {
		parseStatus = fgets(line, 100, filesys);
		while(parseStatus != NULL) {
			if(strlen(line) > 1) { //the only line that matches this is root, and that is already taken care of with init()
				line[strlen(line) - 1] = '\0'; //chop off /n from the end of the line
				type = line[0]; //get type
				strcpy(line, chopNChars(line, 3)); //Chop off leading type char and the space
				printf("recreating \"%s\" \n", line);

				if(type == 'D') {
					mkdir(line);
				} else {
					creat(line);
				}

			}
			parseStatus = fgets(line, 100, filesys);
		}
	}

	fclose(filesys);

	return 0;
}

int quit(char filename[]) {
	//no filename is usually passed, so it is used as placeholder so it has the same signiture
	save(filename); //so I don't reinvent the wheel
	return 0;
}

int removefile(char pathname[], char filetype) {
	//If the path is valid, check if basename exists and is a dir
		//if so, check if there are any child points
			//if no, we can delete
				//go to parent, and if it is the childptr, set sibling pointer to basename's sibling
				//delete node
			//if yes, then print file can't be deleted
	char* temp = "";
	
	char basename[64] = "",
		dirname[64] = "";

	Node *parentDir,
			*doomedFile,
			*sibling;

	if(strlen(pathname) == 0) {
		printf("missing pathname/filename\n");
		return 0;
	} else {
		//parse path
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

		doomedFile = search(parentDir, &basename, filetype);

		if(doomedFile == NULL) {
			printf("Specified file does not exist\n");
		} else {
			if(doomedFile->childPtr == NULL) {
				if(parentDir->childPtr == doomedFile) { //is the current file the first child?
					parentDir->childPtr = doomedFile->siblingPtr;
				}
				free(doomedFile); //finaly delete the node
			} else {
				printf("Cannot delete directory that contains files\n");
			}
		}
	}
	return 0;
}

int make(char pathname[], char filetype) {
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
	char* temp = "";
	
	char basename[64] = "",
		dirname[64] = "";

	Node *parentDir,
		 *newFile,
		 *oldestSibling;


	if(strlen(pathname) == 0) {
		printf("missing pathname/filename\n");
	} else {
		temp = getBaseName(pathname); //parsing path
		if(temp == NULL) {
			strcpy(basename, pathname);
		} else {
			strcpy(basename, temp);
		}

		temp = getDirName(pathname);
		if(temp != NULL) strcpy(dirname, temp);
		if(strlen(dirname) > 0) { //if we have a path, we will follow it to the parent node
			parentDir = followPath(dirname);
		} else {
			parentDir = cwd;
		}
		if(parentDir != NULL && search(parentDir, &basename, 'A') == NULL) { //Make sure we have a parent and filename doesn't already exist
			newFile = (Node *)malloc(sizeof(Node));
			newFile->type = filetype;
			strcpy(newFile->name, basename); //want to make sure Node holds the data
			newFile->siblingPtr = newFile->childPtr = NULL; //Always null at the start

			if(parentDir->childPtr == NULL) { //Only set if this is the parents first child
				parentDir->childPtr = newFile;
			} else { //if it has children, then we will have a sibling
				oldestSibling = getLastChild(parentDir);
			}

			newFile->parentPtr = parentDir; //all sibling's parent is the parentDir

			if(oldestSibling != NULL) { //If we have a sibling, set
				oldestSibling->siblingPtr = newFile; //Sets sibling pointer so it can be accessed again
			}
		} else {
			printf("Invalid directory or filename\n");

		}
	}
	return 0;
}

Node* search(Node* parent,char *basename, char filetype) {
	/*Searches a parent directory for a given file name of a certain file type
		File types:
			F - Look only for files
			D - Look only for directories
			A - Look for both

		Goes to child (so first child)
		While I have a valid siblingPtr, check if the name and filetype match
			If I find a match, return it
			If I get past all valid children, then return NULL

	*/
	Node *current = parent->childPtr; //Get to the childptr to look for those
	if(current != NULL) {
		//Go to parent, return to first child. Makes sure we have the first sibling
		//Node* current = (startingPtr->parentPtr)->childPtr;
		if(current == NULL) {
			return NULL;
		} else if(strcmp(current->name, "/") == 0) {
			return NULL; //We won't find anything if we try to search from root as it's sibling pointer is itself
		} else if(strcmp(basename, current->name) == 0 && filetype == current->type) { //if the first child is what we are looking for
			return current;
		} else {
			do {
				if(strcmp(current->name, basename) == 0) {
					if(filetype == current->type || filetype == 'A') { //Checks if type is correct. 'a' flag denotes any type is fine
						return current;
					}
				}
				current = current->siblingPtr;
			} while(current != NULL);
		}
	}

	//we are out of the loop here, AKA we didn't find it
	return NULL;
}

char* getBaseName(char path[]) {
	/*
		Gets the last token of a path (not including the slash)
		if there is not a '/' in the path, return null. I tried returning the path, but I had issues with that
	*/
	char* lastSlash = strrchr(path, '/');
	if(lastSlash == NULL) { //there is not path besides the file name
		return NULL;
	} else {
		return chopNChars(lastSlash, 1);
	}
	printf("lastSlash=%s\n", lastSlash);
	return lastSlash;
}

char* getDirName(char path[]) {
	/*
		gets everything before the last '/'
		if there is no slash, this returns null
	*/
	int path_length = strlen(path),
		basename_length = 0;
	char* end = strrchr(path, '/');
	char* _dirname = malloc(sizeof(char) * 64);
	if(end == NULL) {
		//if there is the dirname
		return NULL;
	}
	basename_length = strlen(end);
	strncpy(_dirname, path, (path_length - basename_length));  //the minus one is to knock off the additional slash
	return  _dirname;
}

//Scans until the last dir and returns that so we can easily add dirs to the end
Node* getLastChild(Node* dir) {
	Node* current = dir;
	if(current->childPtr == NULL) {
		return NULL;
	} else if(current->childPtr != NULL) {
		current = current->childPtr;
		while(current->siblingPtr != NULL) {
			current = current->siblingPtr;
		}	
	}
	return current;
}

Node* followPath(char path[]) {
	//Dirs holds each dir in the path
	char *dirs[64]; //Techincal limit of 32 for dirname and basename, round up to be safe. 
	char backup[64]; //So I don't destroy the path by accident
	char *temp;
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
	if(strlen(path) == 1 && path[0] == '/') { //if only a single slash is passed it, we go to the root directory
		return root; 
	} else if(path[0] == '/') { //absolute path
		current = root->childPtr;
	} else {
		current = cwd;
	}
	if(current != NULL) {
		printf("following path from: %s\n", current->name);

		//Initialize dirs array
		for(i = 0; i < 64; i++) {
			dirs[i] = malloc(64 * sizeof(char)); //allocate max of 64 chars in the array
		}
		if(strchr(backup, '/') == NULL) { //looks for a slash (sign of multiple dirs) only 1 dir deep
			strcpy(dirs[0], backup);
			numDirs = 1;
		} else {
			temp = strtok(backup, " /");
			i = 0;

			while(temp != NULL) { //adds each dir to the array
				dirs[i] = temp;
				i++;
				numDirs++;
				temp = strtok(NULL, " /");
			}
			/*for (i = 0; i < numDirs; i++)
			{
				printf("%s, ", dirs[i]);
			}
			printf("\n");*/
		}

		//scans while we have not found it, and while the path exists
		i = 0;
		while(valid && i < numDirs) { //While we haven't run into a non-existant file, and we aren't at the end of the path
			if(strcmp(dirs[i], "..") == 0) { //Hard coded move to parent
				current = current->parentPtr;
			} else {
				current = search(current, dirs[i], 'D'); //Look for specified directoy
			}
			if(current == NULL) {
				valid = 0;
				return NULL;
			}
			i++;
		}
		return current;
	} else {
		return NULL;
	}
}

//Function to chop off characters from the front of a string. Mostly used to remove the leading slash
char* chopNChars(char str[], int chars) {
	int str_len = strlen(str);
	char* temp = malloc(sizeof(char) * 64);
	char newStr[64];

	if(chars > str_len) { //Don't even try if I am supposed to chop off more characters than I have
		puts("You are asking to take too many chars off");
		return str;
	} else {
		strncpy(temp, str + chars, str_len - chars);
		printf("newStr = \"%s\"\n", temp);
		return temp;
	}
}

void initialize() {
	root = (Node*)malloc(sizeof(Node));
	strcpy(root->name, "/"); //Hard coded '/' name
	root->type = 'D';
	root->childPtr = NULL;
	root->siblingPtr = root; //As specified by lab1 doc
	root->parentPtr = root;
	cwd = root;
}