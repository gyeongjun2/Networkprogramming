#include "inet.h"
#include <sys/select.h>
#include <sys/time.h>

#define MSGLEN 1024
#define MAXCLI 50

int main(int argc, char *argv[]){
		
	int sockfd, confd;
	struct sockaddr_in cliaddr, servaddr;
	int addrlen;
	fd_set read_fdset;
	int client_fd[MAXCLI];
	int client_num = 0;
	char buf[MSGLEN];
	int msg_len, maxfdp1, nready, i;
	pid_t childpid;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("server: cant create socket\n");
		exit(-1);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	if((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))<0){
		printf("server: bind error\n");
		exit(-2);
	}
	listen(sockfd, 5);


	for(i=0; i< MAXCLI; i++){
		client_fd[i] = -1;
		}
	FD_ZERO(&read_fdset);
	maxfdp1 = sockfd + 1;
	
	while(1){
		FD_SET(sockfd, &read_fdset);
		for(i = 0; i < MAXCLI; i++){
			if(client_fd[i]!=-1)
				FD_SET(client_fd[i], &read_fdset);
		}

		if((nready = select(maxfdp1, &read_fdset, NULL,NULL,NULL))<0){
			printf("select error..\n");
			exit(-1);
		}

		if(FD_ISSET(sockfd, &read_fdset)){
			addrlen = sizeof(cliaddr);
			confd = accept(sockfd, (struct sockaddr *)&cliaddr, &addrlen);
			if(confd <0){
				printf("server: accept error\n");
				exit(-1);
			}
			if(client_num>=MAXCLI){
				printf("too many clients\n");
				close(confd);
				continue;
			}
			printf("connection new client(confd=%d)...\n", confd);

			for(i=0; i<MAXCLI; i++){
				if(client_fd[i] !=-1)
					continue;
				client_fd[i] = confd;
				client_num++;
				maxfdp1++;
				break;
			}
		}
		
		for(i=0;i<MAXCLI;i++){

			if(client_fd[i]==-1)
				continue;
			if(FD_ISSET(client_fd[i], &read_fdset)){
				memset(buf, '\0', sizeof(buf));
				msg_len = read(client_fd[i], buf, MSGLEN);
				if(write(client_fd[i], buf, msg_len)!=msg_len){
					printf("server: write error\n");
					close(client_fd[i]);
					exit(0);
				}
			}
			if(--nready<=0)
				break;
		}
	}
}
