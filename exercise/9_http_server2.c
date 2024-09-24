#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 4096

void parse_url(const char *url, char *host, char *file, int *port);
void send_request(int sock, const char *host, const char *file);
void receive_and_save(int sock, const char *filename);
void spawn_viewer(const char *filename);


int main(int argc, char *argv[]){

	if(argc!=2){
		printf("Usage: %s URL\n",argv[0]);
		exit(1);
	}

	char host[256], file[256];
	int port = 80;
	parse_url(argv[1], host, file, &port);

	struct hostent *server = gethostbyname(host);
	if(server==NULL){
		fprintf(stderr,"Error: No such host\n");
		exit(2);
	}

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock==-1){
		printf("socket() error");
		exit(3);
	}

	struct sockaddr_in serv_addr;
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
		printf("connect() error");
		close(sock);
		exit(4);
	}

	send_request(sock, host, file);

	char filename[256];
	sprintf(filename, "/tmp/%s",strrchr(file,'/') ? strrchr(file,'/')+1:file);
	receive_and_save(sock, filename);

	spawn_viewer(filename);
	
	close(sock);
	return 0;

}


void parse_url(const char *url, char *host, char *file, int *port){

	sscanf(url, "http://%[^:/]:%d%s", host, port, file);

	if(*port==0)
		*port = 80;
	
	if(strlen(file)==0)
		strcpy(file,"/index.html");

}

void send_request(int sock, const char *host, const char *file){

	char request[BUFFER_SIZE];
	sprintf(request, "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", file, host);
	write(sock, request, strlen(request));
}


void receive_and_save(int sock, const char *filename){

	char buffer[BUFFER_SIZE];
	int n;
	int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0644);
	if(fd<0){
		printf("open() error");
		exit(5);
	}

	while((n = read(sock, buffer, BUFFER_SIZE))>0){
		write(fd, buffer, n);
	}
	close(fd);
}


void spawn_viewer(const char *filename){

	if(strstr(filename, ".html")!=NULL){
		execlp("lynx", "lynx", filename, NULL);
	}else if(strstr(filename, ".gif")!=NULL){
		execlp("xv", "xv", filename, NULL);
	}else{
		execlp("less", "less", filename, NULL);
	}
}










