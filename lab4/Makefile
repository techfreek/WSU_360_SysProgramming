all: client server

client: client.c netlib.c
	gcc -m32 -o client client.c netlib.c -I. -g -w

server: server.c netlib.c
	gcc -m32 -o server server.c netlib.c -I. -g -w