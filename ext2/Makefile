#lab: fileops.c main.c minode.c mount.c proc.c utility.c mk.c rm.c
	#gcc  -o lab fileops.c main.c minode.c mount.c proc.c utility.c mk.c rm.c -I. -g -w

CC=gcc
CFLAGS=-I. -g -w

SRC=$(wildcard *.c)

lab: $(SRC)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
