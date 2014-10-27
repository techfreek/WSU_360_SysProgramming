#include "proc.h"

PROC *running = NULL;
int numProcesses = 0;

//	How can proc be a linked list if I don't have a 'struct proc *next' variable?


int newProc(int uid) {
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
	process->fd = NULL;
	return 1;
}