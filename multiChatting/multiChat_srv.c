#include "chatdr_srv.h"

int main(int argc, char *argv[]){

	int sockfd, port;
	struct sockaddr_in cli_addr, serv_addr;
	printf("Input port number : ");
	if(scanf("%d", &port)!=1)
		errmsg("server : port number input error!!");

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		errmsg("server : cant open stream socket");
	}

	//clear serv_addr
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if(bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
		errmsg("server : cant bind local address");
	}

	listen(sockfd, 5);
	acceptcli(sockfd);

	close(sockfd);
}
