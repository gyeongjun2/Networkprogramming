#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

	struct in_addr ip;
	struct servent *service;
	int port;

	if(argc !=3){

		printf("Usage: getservice [service][port]\n");
		exit(0);
	}

	if((service = getservbyname(argv[1], NULL))==NULL){
		printf("getservbyname error\n");
		exit(-1);
	}

	port = ntohs(service->s_port);
	printf("%s port number: %d(%s)\n", service->s_name, port, service->s_proto);

	memset(service, 0, sizeof(struct servent));
	port = htons(atoi(argv[2]));
	if((service = getservbyport(port, NULL))==NULL){
		printf("getservbyport error\n");
		exit(-1);
	}
	printf("port %d(%s) : %s service\n", ntohs(service->s_port), service->s_proto, service->s_name);


}
