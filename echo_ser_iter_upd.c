#include "inet.h"
#define MSGLEN 128

int main(int argc, char *argv[]){

	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	int addrlen, msg_len;

	char clidot[20], buf[MSGLEN];
	time_t date;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	while(1){
		memset(buf, '\0', sizeof(buf));
		addrlen = sizeof(cliaddr);
		msg_len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &addrlen);
		if(msg_len<0)
			continue;

		printf("client: %s, port %d msg_len: %d\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, clidot, sizeof(clidot)),
				ntohs(cliaddr.sin_port), msg_len);
		if(sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr))!= msg_len)
			printf("sendto error\n");
	}
}
