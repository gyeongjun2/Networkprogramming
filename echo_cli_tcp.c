#include "inet.h"
#define MSGLEN 1024

int main(int argc, char *argv[]){

	int sockfd;
	struct sockaddr_in servaddr;

	char buf[MSGLEN];
	int len, msg_len, rcvlen;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi(argv[2]));

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	while(1){
		memset(buf, '\0', sizeof(buf));
		if((msg_len = read(0, buf, sizeof(buf)))<=0){
			close(sockfd);
			exit(0);
		}

		if(write(sockfd, buf, msg_len)!=msg_len){
			printf("client: write error!!\n");
			close(sockfd);
			exit(0);
		}

		memset(buf, '\0', sizeof(buf));

		len = 0;
		do{
			if((rcvlen = read(sockfd, &buf[len], msg_len-len))<=0){
				close(sockfd);
				exit(0);
			}
			len += rcvlen;
		}while(len<msg_len);
		printf("echo : %s", buf);
	}


}
