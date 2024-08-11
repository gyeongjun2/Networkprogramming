#include "inet.h"
#include <time.h>
#define MSGLEN 128

int main(int argc, char *argv[]){

	int sockfd, clifd;
	struct sockaddr_in servaddr;
	int len;
	char buf[MSGLEN];
	time_t date;

	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
		printf("client: cant connect\n");
		exit(-1);
	}

	memset(buf, '\0', sizeof(buf));
	while((len = recv(sockfd, buf, MSGLEN, 0))>0){
		buf[len] ='\0';
		printf("%s", buf);
	}
	close(sockfd);

}
