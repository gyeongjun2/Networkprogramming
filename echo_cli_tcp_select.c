#include "inet.h"
#include <sys/select.h>
#include <sys/time.h>

#define MSGLEN 1024

int main(int argc, char *argv[]){

	int sockfd;
	struct sockaddr_in servaddr;
	fd_set read_fdset, reset;
	char buf[MSGLEN];
	int len, msg_len, rcvlen, maxfdp1;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("clinet: cant create socket\n");
		exit(-1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0){
		printf("cli: cant connect\n");
		exit(-1);
	}

	FD_ZERO(&read_fdset);
	FD_SET(0, &read_fdset);
	FD_SET(sockfd, &read_fdset);
	reset = read_fdset;
	maxfdp1 = sockfd + 1;

	while(1){
		read_fdset = reset;
		if(select(maxfdp1, &read_fdset, NULL,NULL,NULL)<0){
			printf("select error..\n");
			exit(-1);
		}
		if(FD_ISSET(0,&read_fdset)){
			memset(buf,'\0', sizeof(buf));
			if((msg_len = read(0, buf, sizeof(buf)))<=0){
				close(sockfd);
				exit(0);
			}
			if(write(sockfd, buf, msg_len)!=msg_len){
				printf("cli: write error\n");
				close(sockfd);
				exit(0);
			}
		}
		if(FD_ISSET(sockfd, &read_fdset)){
			memset(buf, '\0', sizeof(buf));
			len = 0;

			do{
				if((rcvlen = read(sockfd, &buf[len], msg_len - len))<=0){
					close(sockfd);
					exit(0);
				}
				len += rcvlen;

			}while(len<msg_len);
			printf("echo: %s", buf);
		}

	}
}
