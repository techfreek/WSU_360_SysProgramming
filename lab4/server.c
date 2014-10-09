// This is the echo SERVER server.c
#include "netlib.h"

void readCommand(char cmd[]);

// Define variables:
struct sockaddr_in	server_addr, client_addr, name_addr;
struct hostent *hp;

extern int	sock, newsock;									// socket descriptors
int	serverPort;										 // server port number
int	r, length, n;									 // help variables

extern char* cwd;
extern char* rootcwd;

// Server initialization code:


int server_init(char *name)
{
	cwd = (char*)malloc(sizeof(char) * 128);
	getcwd(cwd, 128);
	rootcwd = strdup(cwd);
	
	printf("==================== server init ======================\n");	 
	 // get DOT name and IP address of this host

	printf("1 : get and show server host info\n");
	hp = gethostbyname(name);
	if (hp == 0){
		printf("unknown host\n");
		exit(1);
	}
	printf("		hostname=%s	IP=%s\n",
			hp->h_name,	inet_ntoa(*(long *)hp->h_addr));
	
	//	create a TCP socket by socket() syscall
	printf("2 : create a socket\n");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		printf("socket call failed\n");
		exit(2);
	}

	printf("3 : fill server_addr with host IP and PORT# info\n");
	// initialize the server_addr structure
	server_addr.sin_family = AF_INET;									// for TCP/IP
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	 // THIS HOST IP address	
	server_addr.sin_port = 0;	 // let kernel assign port

	printf("4 : bind socket to host info\n");
	// bind syscall: bind the socket to server_addr info
	r = bind(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
	if (r < 0){
		printf("bind failed\n");
		exit(3);
	}

	printf("5 : find out Kernel assigned PORT# and show it\n");
	// find out socket port number (assigned by kernel)
	length = sizeof(name_addr);
	r = getsockname(sock, (struct sockaddr *)&name_addr, &length);
	if (r < 0){
		printf("get socketname error\n");
		exit(4);
	}

	// show port number
	serverPort = ntohs(name_addr.sin_port);	 // convert to host ushort
	printf("		Port=%d\n", serverPort);

	// listen at port with a max. queue of 5 (waiting clients) 
	printf("5 : server is listening ....\n");
	listen(sock, 5);
	printf("===================== init done =======================\n");
}


int main(int argc, char *argv[])
{
	char *hostname;
	char line[NETTRANS];

	if (argc < 2) {
		hostname = "localhost";
	} else {
		hostname = argv[1];
	}
 
	server_init(hostname); 

	// Try to accept a client request
	while(1){
		printf("server: accepting new connection ....\n"); 

		// Try to accept a client connection as descriptor newsock
		length = sizeof(client_addr);
		newsock = accept(sock, (struct sockaddr *)&client_addr, &length);
		if (newsock < 0){
			printf("server: accept error\n");
			exit(1);
		}
		printf("server: accepted a client connection from\n");
		printf("-----------------------------------------------\n");
		printf("				IP=%s	port=%d\n", inet_ntoa(client_addr.sin_addr.s_addr),
											ntohs(client_addr.sin_port));
		printf("-----------------------------------------------\n");

		// Processing loop
		while(1){
			n = read(newsock, line, NETTRANS);
			if (n==0){
				printf("server: client died, server loops\n");
				close(newsock);
				break;
			}

			if(isCommand(line)) { //if command
				readCommand(line);
				char *function = getFunction(line);
				char *pathname = getPath(line);
				char *filename = getFilename(line);
				
				if(strncmp(line, "::DONE", 6) == 0) {
					//do nothing
					//sometimes an error comes up that sends this
				} else if(filename) {
					transfer(filename, newsock);
				} else {
					int func = functionLookup(function);
					printf("Function ID: %d\n", func);
					if(func >= 0) {
						callFunction(func, pathname, newsock);
					} else {
						printf("Could not find command: %s\n", function);
						endCommunication(newsock);
					}
				}
			} else {
				printf("%c\n", line);
			}

			printf("server: ready for next request\n");
		}
 	}
}

void readCommand(char cmd[]) {
	char *funcPos = getFunction(cmd),
		 *pathPos = getPath(cmd),
		 *fileName = getFilename(cmd),
		 *content = getContent(cmd);

	int filesize = getFilesize(cmd),
		linesize = getLinesize(cmd);

	if(funcPos) {
		printf("Function: %s\n", funcPos);
	}

	if(pathPos) {
		printf("Path: %s\n", pathPos);
	}

	if(filesize > 0) {
		printf("filesize: %d\n", filesize);
	}

	if(fileName) {
		printf("fileName: %s\n", fileName);
	}

	if(linesize > 0) {
		printf("lineSize: %d\n", linesize);
	}

	if(content) {
		printf("content: %s\n", content);
	}

}

/*
	::filesize=(size)&name=(filename)
	ex ::filsize=9001&name=overninethousand.txt

	//file line
	::linesize=(size)&content=(content)
*/

int isCommand(const char line[]) {
	if(line[0] == ':' && line[1] == ':') {
		return 1;
	} else {
		return 0;
	}
}