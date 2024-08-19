#include "chathdr_cli.h"

int main(int argc, char *argv[]){

	int sockfd, port;
	char alias[30], host[50];

	struct sockaddr_in serv_addr;
	struct hostent *hostname;
	struct in_addr in;

	printf("Input port number:");
	scanf("%d", &port);

	while(getchar()!='\n');

	do{
		printf("Input alias : ");
		fgets(alias, sizeof(alias), stdin);
	}while(strlen(alias)-1 <=0);

	alias[strlen(alias)-1] = '\0';
	hostname = gethostbyname(argv[1]);
	memcpy(&in.s_addr, hostname->h_addr_list[0], sizeof(in.s_addr));

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = in.s_addr;
	serv_addr.sin_port = htons(port);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		errmsg("client : cant open stream socket");

	if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		errmsg("client : cant connect to server");
	
	doChat(sockfd, alias);

	close(sockfd);


}
