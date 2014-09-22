#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char *argv[ ])
{
	printf("b.out parent=%d", getppid());
	printf("proc %d in b.out: argc=%d\n", getpid(), argc);
	int i;
	for(i = 0; i < argc; i++) {
		printf("argv[%d] = %s\n", i, argv[i]);
	}

	printf("proc %d in b.out exit\n", getpid());
}