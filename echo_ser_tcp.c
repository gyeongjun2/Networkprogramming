#include "inet.h"
#define MSGLEN 1024

int main(int argc, char *argv[]){

	int sockfd, confd;
	struct sockaddr_in cliaddr, servaddr;	//stored client&server addr info
	int addrlen;
	char buf[MSGLEN];
	int msg_len;
	pid_t childpid;

	if(argc !=2){
		printf("Usage : echo_ser [Port number]\n");
		exit(0);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("server: cant create socket\n");
		exit(-1);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
		printf("server: bind error\n");
		exit(-1);
	}

	listen(sockfd, 5); //max 5 client

	//wait client
	while(1){
		addrlen = sizeof(cliaddr);
		confd = accept(sockfd, (struct sockaddr*) &cliaddr, &addrlen);
		if(confd<0){
			printf("server: accpet error\n");
			exit(-1);
		}
		printf("connection new client(confd=%d)...\n", confd);

		if((childpid=fork())==0){	//child
			close(sockfd);
			while(1){
				memset(buf, '\0', sizeof(buf));
				msg_len = read(confd, buf, MSGLEN);
				if(write(confd, buf, msg_len)!=msg_len){
					printf("server: write error!!\n");
					close(confd);
					exit(0);
				}
			}
		}
		close(confd);
	}
}
