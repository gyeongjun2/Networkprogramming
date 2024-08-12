#include "inet.h"
#include <sys/select.h>
#include <sys/time.h>

#define MSGLEN 128

int main(int argc, char *argv[]){

	int sockfd, clifd;
	struct sockaddr_in servaddr;
	fd_set read_fdset, reset;
	char buf[MSGLEN];
	int addrlen, len, msg_len, maxfdp1;
	time_t date;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	FD_ZERO(&read_fdset);
	FD_SET(0, &read_fdset);
	FD_SET(sockfd, &read_fdset);
	reset = read_fdset;
	maxfdp1 = sockfd + 1;

	while(1){
		read_fdset = reset;
		if(select(maxfdp1, &read_fdset, NULL,NULL,NULL)<0){
			printf("select error\n");
			exit(-1);
		}
		if(FD_ISSET(0, &read_fdset)){
			memset(buf, '\0', sizeof(buf));
			if((msg_len = read(0, buf, sizeof(buf)))<0){
				close(sockfd);
				exit(0);
			}
			if(sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr))!=msg_len){
				printf("sendto error");
				close(sockfd);
				exit(-1);
			}
		}
		if(FD_ISSET(sockfd, &read_fdset)){
			memset(buf,'\0', sizeof(buf));

			addrlen = sizeof(servaddr);
			len = recvfrom(sockfd, buf, MSGLEN, 0, (struct sockaddr *) &servaddr, &addrlen);
			printf("echo: %s", buf);

		}
	}
}
