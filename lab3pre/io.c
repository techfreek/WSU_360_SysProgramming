#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

main()
{
	int fd;
	printf("this is PROC %d printing to stdout\n", getpid());

	fd = open("myfile", O_WRONLY|O_CREAT, 0644);
	printf("fd = %d\n", fd);

	close(1);
	dup(fd);

	getchar();

	printf("where does this line go?\n");
	printf("and this line?\n");

}