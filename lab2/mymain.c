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

	myprintf("--------------testing YOUR myprintf() ---------------\n");
	myprintf("this is a test\n");
	myprintf("This is another test=%d\n", 9);
	myprintf("testing a=%d b=%x c=%c s=%s\n", 123, 123, 'a', "testing");
	myprintf("string=%s, a=%d  b=%u  c=%o  d=%x\n", "testing string", -1024, 1024, 1024, 1024);
	myprintf("mymain() return to main() in assembly\n"); 
}