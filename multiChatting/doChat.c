#include "chathdr_cli.h"

void doChat(int sockfd, char *alias){

	int msglen, pid;
	char sndmsg[MAXMSG], recvmsg[MAXMSG];
	fd_set read_fdset, reset;
	int maxfdp1;

	if(write(sockfd, alias, strlen(alias)+1)!=strlen(alias)+1)
		errmsg("client : alias send error");

	printf("\nClose Chat : type \"quit\"\n\n");

	FD_ZERO(&read_fdset);
	FD_SET(0, &read_fdset);
	FD_SET(sockfd, &read_fdset);
	reset = read_fdset;
	maxfdp1 = sockfd + 1;

	signal(SIGINT, SIG_IGN);

	while(1){

		fprintf(stderr,"[ %-10s ] : ", alias);
		read_fdset = reset;

		if(select(maxfdp1, &read_fdset, NULL,NULL,NULL)<0){
			printf("select error");
			exit(-1);
		}

		if(FD_ISSET(0, &read_fdset)){
			memset(sndmsg, '\0', sizeof(sndmsg));
			fgets(sndmsg, sizeof(sndmsg), stdin);
			sndmsg[strlen(sndmsg)-1] = '\0';
			msglen = strlen(sndmsg);
			if(write(sockfd, sndmsg, msglen) != msglen)
				errmsg("client : message send error");
			if(!strcmp(sndmsg, "quit")){
				printf("---bye---\n");
				return;
			}
		}
		if(FD_ISSET(sockfd, &read_fdset)){
			memset(recvmsg, '\0', sizeof(recvmsg));
			if(read(sockfd, recvmsg, MAXMSG+1)<=0)
				errmsg("client : meesage receive error");

			msglen = strlen(recvmsg);
			recvmsg[msglen] = '\0';
			printf("\n%s\n", recvmsg);
		}
	}
}

