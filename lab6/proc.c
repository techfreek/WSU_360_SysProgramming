#include "proc.h"

PROC *running = NULL; //move to an global variable
extern MINODE *root;

int numProcesses = 0;

PROC* newProc(int uid) {
	PROC *process = (PROC*)malloc(sizeof(PROC));
	if(process == NULL) {
		return 0;
	}

	//initialize variables
	process->uid = uid;
	process->pid = numProcesses++; //auto increment
	process->gid = 0;
	process->status = READY;
	printf("Setting proc cwd\n");
	process->cwd = root;
	printf("Setting devId with %d\n", root->dev);
	process->cwdDevId = getDevID(root->dev);

	OFT files[NFD];

	int i = 0;
	for (i = 0; i < NFD; ++i)
	{
		files[i].mode = 0;
		files[i].refCount = 0;
		files[i].inodeptr = NULL;
		files[i].offset = 0;
	}

	*process->fd = files;

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

void killAllProcs() {

}

void printAllProcs() {
	PROC *start = running;
	PROC *curr = running;
	int i = 0;

	printf("\n%%%%%%%%%% Procs %%%%%%%%%%\n");
	do {
		printf("\nUID | PID | GID | STATUS | devID \n");
		printf("%3d | %3d | %3d | %6d | %d\n", curr->uid, curr->pid, curr->gid, curr->status, curr->cwdDevId);

		printf("\nCWD: \n");
		printMINode(curr->cwd);

		printf("\nOpen files: \n");
		printf("i |  mode  | refCount | offset \n");
		printf("-------------------------------\n");
		for(i = 0; i < NFD; i++) {
			if(curr->fd[i] != NULL && curr->fd[i]->refCount > 0) {
				printf("%d | %6d | %8d | %d\n", i, 
					curr->fd[i]->mode, curr->fd[i]->refCount, /*curr->fd[i]->inodeptr->ino,*/ curr->fd[i]->offset);
			}
		}

		printf("-------------------------------\n");
		curr = curr->next;
	} while ((curr != NULL) && (curr != start));
	printf("%%%%%%%%%%  END PROCS %%%%%%%%%%\n\n");
}
