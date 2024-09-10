#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main(int argc, char *argv[]){

	int sock;
	struct sockaddr_in server_addr;
	char buffer[SIZE];
	int fd;

	if(argc!=3){
		printf("Usage: %s <server-ip> <filename>\n", argv[0]);
		exit(1);
	}


	sock = socket(AF_INET,SOCK_STREAM, 0);

	if(sock==-1){
		printf("socket() error");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(PORT);

	if(connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr))==-1){
		printf("connect() error");
		exit(2);
	}

	write(sock, argv[2], strlen(argv[2]));

	fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0644);
	
	int read_len;
	while((read_len=read(sock, buffer, SIZE))>0){
		write(fd, buffer, read_len);	
	}

	close(fd);
	close(sock);
	return 0;

}
