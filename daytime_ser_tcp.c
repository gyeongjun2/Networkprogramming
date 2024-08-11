#include "inet.h"
#include <time.h>

#define MSGLEN 128

int main(int argc, char *argv[]){

	int sockfd, confd;
	struct sockaddr_in servaddr, cliaddr;

	int len;
	char buf[MSGLEN];
	time_t date;

	if(argc !=2){
		printf("Usage: daytime_ser[Port number]\n");
		exit(0);
	}

	//if error ret -1
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){	//AF_INET : IPv4, SOCK_STREAM : TCP
		printf("server: cant create socket\n");
		exit(-1);
	}

	servaddr.sin_family = AF_INET;	//IPv4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //wild card	
	servaddr.sin_port = htons(atoi(argv[1]));

	if((bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)))<0){
		printf("server: bind error\n");
		exit(-1);
	}

	listen(sockfd, 5);

	while(1){
		len = sizeof(cliaddr);
		confd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
		if(confd<0){
			printf("server: accept error\n");
			exit(-1);
		}

		printf("connection from %s, port %d\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)),
				ntohs(cliaddr.sin_port));

		date = time(NULL);
		memset(buf, '\0', sizeof(buf));
		sprintf(buf, "%s", ctime(&date));
		send(confd, buf, strlen(buf), 0);
		close(confd);
	}

}
