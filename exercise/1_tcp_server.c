#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 8080
#define SIZE 1024

int main(){

	int server_sock, client_sock;
	struct sockaddr_in serv_addr, client_addr;
	socklen_t client_addr_size;
	char buffer[SIZE];
	int fd;

	//1. create socket
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(server_sock==-1){
		printf("socket() error");
		exit(1);
	}

	//2. setting serv_addr
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

	//3. socket bind
	if(bind(server_sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1){
		printf("bind() error");
		exit(2);
	}

	//4. wait client
	listen(server_sock,5);

	//5. accept client
	client_addr_size = sizeof(client_addr);
	client_sock = accept(server_sock,(struct sockaddr *)&client_addr, &client_addr_size);

	//6. read filename from client
	int str_len = read(client_sock, buffer, SIZE-1);

	if(str_len==-1){
		printf("read error");
		exit(4);
	}
	//notice end
	buffer[str_len] = '\0';

	fd = open(buffer, O_RDONLY,0666);
	if(fd==-1){
		printf("File not found\n");
		close(server_sock);
		close(client_sock);
		exit(3);
	}

	//write file data to client_sock -> data going to client
	int read_len;
	while((read_len=read(fd, buffer, SIZE))>0){
		write(client_sock, buffer, read_len);
	}

	close(fd);
	close(client_sock);
	close(server_sock);

}
