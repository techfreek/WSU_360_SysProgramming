int mkdir(char *pathname) {
	//parse path
		//if relative, follow path while checking if dir exists and is actually a dir
		//if absolute, scan path from root	
	//If the path is valid, check to make sure there is not another file with the same name
		//if so, then we create a new node
			//set child and sibling pointers to null,
			//parentptr set to last sibling
}

int rmdir(char *pathname) {
	//parse path
		//if relative, follow path while checking if dir exists and is actually a dir
		//if absolute, scan path from root	
	//If the path is valid, check if basename exists and is a dir
		//if so, check if there are any child points
			//if no, we can delete
				//go to parent, and set sibling pointer to basename's sibling
				//delete node
			//if yes, then print file can't be deleted
}

int cd(char *pathname) {
	//parse path
		//if null, set to root
		//if relative, follow path while checking if dir exists and is actually a dir
		//if absolute, scan path from root	
	//if valid and a dir
		//set cwd to that dir
}

int ls(char *pathname) {
	//if null, print this dir
	//if not null,
		//verify path is good
	//if good path/null path
		//go to child ptr
		//print off filename
		//go to sibling
		//print off name
		//repeat
}

int pwd(char *pathname) {
	//ignore pathname
	//call rpwd with cwd node

}

int rpwd(Node *dir) {
	//if dir->parent == dir->child or dir->name == "/" (root)
		//print dir->name
		//return
	//else
		//call rpwd with parent of dir
		//print dir->name
}

int creat(char *pathname) {
	//check if dirname is valid
		//if valid
			//check if there is another node with the same name as the basename
			//if not
				//create new node
				//set type to be 'F'
				//set name to basename
				//parent/child/sibling = null
				//last sibling of directory has sibling ptr pointed at file


}

int rm(char *pathname) {
	//if valid path
		//check if file
		//set previous siblings sibling pointer to basename's sibling ptr
		//delete node

}

int save(char *filename) {
	//file name is what we are saving to
	//open file
	//call rsave with root
	//close file

}

int rsave(Node *file, FILE *fp) {
	//recursive portion of save
	//output type then path to file
	//if file->sibling != null
		//call rsave on file->sibling
	//if file->child != null
		//call rsave on file->child
	//return
}


int reload(char *filename) {
	//open file
	//read line
	//if type == "D" 
		//mkdir with path
	//if type == "F"
		//creat with path
	//close file
}

int quit(char *filename) {
	//no filename passed, used as placeholder so it has the same signiture
	save("bahm_fs.txt");
}