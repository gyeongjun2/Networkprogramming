#include "chatdr_srv.h"

struct cli_info client[MAX_CLI];
int cnum;

void acceptcli(int sockfd){

	int maxfd, i, clilen, newsockfd;
	fd_set read_fds;
	struct sockaddr_in cli_addr;

	clilen = sizeof(cli_addr);
	maxfd = sockfd + 1;
	FD_ZERO(&read_fds);

	printf("Multi chatting server running...\n");

	while(1){
		FD_SET(sockfd, &read_fds);
		for(i=0; i<cnum;i++)
			FD_SET(client[i].clisock, &read_fds);

		if(select(maxfd, &read_fds, (fd_set *)0,(fd_set*)0,(struct timeval*)0)<0)
			errmsg("server : select error");
		if(FD_ISSET(sockfd, &read_fds)){
			if((newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen))<0)
				errmsg("server : client accept error");
			
			add_client(newsockfd);
			maxfd = newsockfd + 1;
			cnum++;
		}
		else{
			for(i = 0; i<cnum; i++)
				if(FD_ISSET(client[i].clisock, &read_fds))
					recv_mesg(i);
		}

	}

}

void add_client(int newsockfd){

	int len;
	char conmesg[MAXMSG];

	memset(conmesg, '\0', sizeof(conmesg));

	client[cnum].clisock = newsockfd;

	if((len = recv(newsockfd, client[cnum].alias, MAXMSG, 0))<0)
		errmsg("server : add_client error!!");
	
	printf("%s join!!\n", client[cnum].alias);
	sprintf(conmesg, "............\"%s\"Join!! :)\0", client[cnum].alias);
	allclisend(cnum, conmesg);

}
