#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define SIZE 1024


int main(int argc, char *argv[]){

	int sock;
	struct sockaddr_in serv_addr;
	char buf[SIZE];
	const char *ping = "ping";
	socklen_t addr_size;
	int str_len;

	if(argc!=2){
		printf("Usage : %s server-ip\n",argv[0]);
		exit(1);
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock==-1){
		printf("socket() error");
		exit(2);
	}

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(PORT);

	addr_size = sizeof(serv_addr);
	//send ping to server
	sendto(sock, ping, strlen(ping), 0, (struct sockaddr*)&serv_addr, addr_size);
	printf("send message complete\n");

	str_len = recvfrom(sock,buf,SIZE,0,NULL,NULL);
	if(str_len<0){
		printf("recvfrom() error");
		exit(3);
	}
	buf[str_len] = '\0';
	
	//printf pong
	printf("%s\n",buf);
	
	close(sock);
	return 0;


}
