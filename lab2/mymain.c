/*
 * Name: Alex Bahm
 * File: mymain.c
 */

#include "myprintf.h"

 mymain(int argc, char *argv[], char *env[]) {
	int i;
	myprintf("in mymain(): argc=%d\n", argc);

	for(i = 0; i < argc; i++) {
		myprintf("argv[%d] = %s\n", i, argv[i]);
	}

	i = 0;
	while(env[i] != 0) {
		myprintf("env[%d] = %s\n", i, env[i]);
		i++;
	}

	myprintf("--------------\\testing YOUR myprintf() ---------------\n");
	myprintf("\tthis is a test\n");
	myprintf("This is another test=%d\n", 9);
	myprintf("testing a=%d b=%x c=%c s=%s\n", 123, 123, 'a', "testing");
	myprintf("string=%s, a=%d  b=%u  c=%d  d=%x\n", "testing string", -1024, 1024, -1024, 1024);
	myprintf("Dec 1024=%d, hex 1024=%x, oct 1024=%o, unsigned 1024=%u\n", 1024, 1024, 1024, 1024);
	myprintf("Dec -1024=%d, hex -1024=%x, oct -1024=%o, unsigned -1024=%u\n", -1024, -1024, -1024, -1024);
	myprintf("mymain() return to main() in assembly\n"); 
}