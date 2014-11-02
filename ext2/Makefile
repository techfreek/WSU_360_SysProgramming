CC=gcc
CFLAGS=-I. -g -w

SRC=$(wildcard *.c)

lab: $(SRC)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
