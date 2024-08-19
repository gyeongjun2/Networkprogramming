#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct cli_info{
	char alias[30];	//nickname
	int clisock;	//cli fd
};

#define MAX_CLI 50 //max client number
#define MAXMSG 256 //max mesg length

#define errmsg(MSG) {printf("%s\n",MSG); exit(1);}

void add_client(int newsockfd);
void acceptcli(int sockfd);
void recv_mesg(int num);
void allclisend(int num, char *message);


