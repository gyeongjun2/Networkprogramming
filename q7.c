#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	struct in_addr ip;	//store ip address
	struct hostent *hostentry;	//pointer to gethostbyaddr, gethostbyname
	char str[50];	//store hostname

	if(argc!=2){
		printf("Usage: address_conversion [IP address ]\n");
		exit(0);
	}

	inet_aton(argv[1], &ip);
	printf("Dotted_decimal to Binary_IP = \t%X\n", ip);

	//ip addr -> domain
	hostentry = gethostbyaddr((char*)&ip, sizeof(ip), AF_INET);
	printf("Binary_IP to Domain_name = \t%s\n", hostentry->h_name);

	//store domain name & memset
	memcpy(str, hostentry->h_name, strlen(hostentry->h_name));	//copy host name to str
	str[strlen(hostentry->h_name)] = '\0';
	memset(hostentry, 0, sizeof(struct hostent));
	memset(&ip, 0, sizeof(ip));

	//domain -> ip addr
	hostentry = gethostbyname(str);
	memcpy(&ip.s_addr, hostentry->h_addr_list[0], sizeof(ip.s_addr));
	printf("Domain_name to Binary_IP = \t%X\n", ip);

	//transform binary to decimal
	printf("Binary_IP to Dotted_decimal = \t%s\n", inet_ntoa(ip));

	


}
