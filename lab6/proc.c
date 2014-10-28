#include "proc.h"

//PROC *running = NULL; //move to an global variable
int numProcesses = 0;

PROC*  newProc(int uid) {
	PROC *process = (PROC*)malloc(sizeof(PROC));
	if(process == NULL) {
		return 0;
	}

	//initialize variables
	process->uid = uid;
	process->pid = numProcesses++; //auto increment
	process->gid = 0;
	process->status = READY
	process->cwd = NULL;

	OFT files[NFD] = {NULL};

	process->fd = files;

	if(running == NULL) {
		process->next = process;
		process->status = RUNNING; //set as the running process
		running = process;
	} else {
		process->next = process->next; //set the next pointer to the link
		running->next = process; //insert the new PROC right after the current running process
	}
	
	return process;
}

int removeProc(int uid) {
	//Search through the running looped linked list. (keep a record of the parent) And watch for the specified UID
	//Use a trailing pointer (always 1 node behind the current PROC)
	//when the uid is found, set the next pointer to the next pointer of the current PROC
	//check to see if there is only proc. If so, set running to 0
}
