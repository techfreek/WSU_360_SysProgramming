//**************************** ECHO CLIENT CODE **************************
// The echo client client.c

#include "netlib.h"

// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 

extern int	sock, newsock;									// socket descriptors
extern char* cwd;

int sock, r;
int SERVER_IP, SERVER_PORT; 

// clinet initialization code

int client_init(char *argv[])
{
	cwd = (char*)malloc(sizeof(char) * 128);
	getcwd(cwd, 128);

	printf("======= clinet init ==========\n");

	printf("1 : get server info\n");
	hp = gethostbyname(argv[1]);
	if (hp==0){
		printf("unknown host %s\n", argv[1]);
		exit(1);
	}

	SERVER_IP   = *(long *)hp->h_addr;
	SERVER_PORT = atoi(argv[2]);

	printf("2 : create a TCP socket\n");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock<0){
		printf("socket call failed\n");
		exit(2);
	}

	printf("3 : fill server_addr with server's IP and PORT#\n");
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = SERVER_IP;
	server_addr.sin_port = htons(SERVER_PORT);

	// Connect to server
	printf("4 : connecting to server ....\n");
	r = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (r < 0){
		printf("connect failed\n");
		exit(1);
	}

	printf("5 : connected OK to \007\n"); 
	printf("---------------------------------------------------------\n");
	printf("hostname=%s  IP=%s  PORT=%d\n", 
					hp->h_name, inet_ntoa(SERVER_IP), SERVER_PORT);
	printf("---------------------------------------------------------\n");

	printf("========= init done ==========\n");
}

int main(int argc, char *argv[ ])
{
	int n;
	char line[NETTRANS], ans[NETTRANS], temp[NETTRANS], cmd[MAX];

	char* path;

	if (argc < 3){
		printf("Usage : client ServerName SeverPort\n");
		exit(1);
	}

	client_init(argv);

	printf("********  processing loop  *********\n");
	while (1){
		printf("input a line : ");
		bzero(line, NETTRANS);                // zero out line[ ]
		fgets(line, NETTRANS, stdin);         // get a line (end with \n) from stdin

		line[strlen(line)-1] = 0;        // kill \n at end
		if (line[0]==0) {                 // exit if NULL line
			exit(0);
		}

		strcpy(temp, strdup(line));
		strcpy(cmd, strtok(temp,  " ")); //get command

		path = strtok(null, " ");

		if(strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) { //'q' is because I am lazy
			exit(0);
		} else if(strlen(cmd) > 2 && cmd[0] == 'l') { //process locally
			int func = functionLookup(&cmd[1]); //Ignore leading l
			if(func >= 0) {
				callFunction(func, path, -1);
			} else {
				printf("Could not find command: %s\n", cmd);
			}
		} else {
			// Send ENTIRE line to server
			bzero(line, NETTRANS); //clear it
			sprintf(line, "::func=%s", cmd);
			if(path != NULL) {
				strcat(line, "&path=");
				strcat(line, path);
			}
			n = write(sock, line, NETTRANS);

			while(strcmp(ans, "::DONE") != 0) {
				n = read(sock, ans, NETTRANS);
				printf("%s\n", ans);
			}
		}


		// Read a line from sock and show it
		n = read(sock, ans, NETTRANS);
		printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);

		if(strncmp("::", line, 2)) { //if command

		} else {
			printf("%c", line);
		}
	}
}

