#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define SIZE 1024

void handle_client(int client_sock);

int main(int argc, char *argv[]){

	int server_sock, client_sock;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t client_addr_size;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock==-1){
		printf("socket() error");
		exit(1);
	}

	memset(&serv_addr, 0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1){
		printf("bind() error");
		exit(2);
	}
	
	listen(server_sock,5);

	while(1){
		client_addr_size = sizeof(cli_addr);

		client_sock = accept(server_sock,(struct sockaddr*)&cli_addr,&client_addr_size);
	
		if(client_sock==-1){
			printf("accept() error");
			continue;
		}

	handle_client(client_sock);	

	}

	close(server_sock);

}

void handle_client(int client_sock){

	char buf[SIZE];
	char method[SIZE], path[SIZE], version[SIZE];
	int fd;

	int read_len = read(client_sock, buf, SIZE-1);
	buf[read_len] = '\0';

	sscanf(buf, "%s %s %s", method, path, version);

	//if not "GET"
	if(strcmp(method, "GET")!=0){
		write(client_sock,"HTTP/1.1 405 Method Not Allowed\r\n\r\n",36);
		close(client_sock);
		return;
	}

	char *filename = path+1; // terminate slash
	
	fd = open(filename, O_RDONLY);
	if(fd==-1){
		char *notFound = "HTTP/1.1 404 Not Found\r\n\r\nFile Not Found";
		write(client_sock, notFound, strlen(notFound));
		close(client_sock);
		return;
	}

	char *ok_response = "HTTP/1.1 200 OK\r\n\r\n";

	write(client_sock, ok_response, strlen(ok_response));

	while((read_len = read(fd,buf,SIZE))>0){
		write(client_sock, buf, read_len);
	}

	close(fd);
	close(client_sock);
}
