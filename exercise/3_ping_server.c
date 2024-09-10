#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main(){

	int server_sock;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t cli_addr_size;
	char buf[SIZE];
	const char *pong = "pong";
	int str_len;
	//create socket
	server_sock = socket(AF_INET,SOCK_DGRAM,0);
	if(server_sock==-1){
		printf("socket() error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

	//bind
	if(bind(server_sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1){
		printf("bind() error");
		exit(2);
		}

	cli_addr_size = sizeof(cli_addr);

	while(1){
		str_len = recvfrom(server_sock,buf,SIZE-1,0,(struct sockaddr*)&cli_addr, &cli_addr_size);
		if(str_len<0){
			printf("recvfrom() error");
			exit(3);
		}
		buf[str_len] = '\0';
		printf("receive from client : %s\n", buf);

		sendto(server_sock,pong,strlen(pong),0,(struct sockaddr*)&cli_addr, cli_addr_size);
		printf("send message(pong)\n");
	}

	close(server_sock);
	return 0;
}
