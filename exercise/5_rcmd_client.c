#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024
//client : socket -> connect -> send -> recv -> close


int my_rcmd(char *srv_addr, int srv_port, char *cmd){

	int sock;
	struct sockaddr_in serv_addr;
	
	sock = socket(AF_INET,SOCK_STREAM, 0);

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(srv_addr);
	serv_addr.sin_port = htons(srv_port);

	connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

	write(sock, cmd, strlen(cmd));
	
	return sock;

}

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("Usage : %s serv_address serv_port command\n",argv[0]);
		exit(1);
	}

	int sock;
	int read_len;
	char buf[SIZE];
	
	sock = my_rcmd(argv[1], atoi(argv[2]),argv[3]);
	if(sock==-1)
		exit(2);
	
	//data receive from server
	while((read_len=read(sock,buf,SIZE))>0){
		write(1,buf,read_len);
	}
	if(read_len==-1)
		perror("read() error");
	
	close(sock);

}
