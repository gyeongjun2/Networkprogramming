#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

	struct hostent *he;
	struct in_addr addr;

	inet_aton(argv[1], &addr);
	he = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	printf("Host name: %s\n", he->h_name);

}
