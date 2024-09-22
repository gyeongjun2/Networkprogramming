#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SIZE 1024

void handle_client(int client_sock);

int main(int argc, char *argv[]){


	int serv_sock, cli_sock;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t cli_addr_size;

	//create socket
	serv_sock = socket(AF_INET,SOCK_STREAM,0);
	if(serv_sock==-1){
		printf("socket() error");
		exit(1);
	}

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
		printf("bind() error");
		exit(2);
	}

	listen(serv_sock, 5);
	
	while(1){
		
		cli_addr_size = sizeof(cli_addr);
		
		//accept
		cli_sock = accept(serv_sock,(struct sockaddr *)&cli_addr,&cli_addr_size);
		if(cli_sock==-1){
			printf("accpet() error");
			exit(3);
		}
		int pid, status=0;

		pid = fork();
		if(pid==0){
			handle_client(cli_sock);
			exit(0);
		}
		close(cli_sock);
		wait(&status);

	}
	close(serv_sock);
}

void handle_client(int client_sock){

	char buf[SIZE];
	char method[SIZE], path[SIZE], protocol[SIZE];
	int fd;
	int read_len = read(client_sock, buf, SIZE-1);
	buf[read_len] = '\0';


	sscanf(buf,"%s %s %s", method, path, protocol);
	
	if(strcmp(method,"GET")!=0){
		write(client_sock, "HTTP/1.1 405 Method Not Allowed\r\n\r\n", 36);
		close(client_sock);
		return;
	}



	
	char *filename = path+1;

	fd = open(filename, O_RDONLY);

	if(fd==-1){

		char *notfound = "HTTP/1.1 404 Not Found\r\n\r\nFile Not Found";
		write(client_sock, notfound, strlen(notfound));
		close(client_sock);
		return;
	}

	char *ok_response = "HTTP/1.1 200 OK\r\n\r\n";
	write(client_sock, ok_response, strlen(ok_response));
	
	while((read_len=read(fd, buf,SIZE))>0){
		write(client_sock, buf, read_len);
	}
	close(fd);
	close(client_sock);

}
