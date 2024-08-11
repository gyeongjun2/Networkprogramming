#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(){

    struct sockaddr_in antelope;
    char *some_addr;

    inet_aton("10.0.0.1", &antelope.sin_addr);

    some_addr = inet_ntoa(antelope.sin_addr);
    printf("%s\n", some_addr);

    antelope.sin_addr.s_addr = inet_addr("10.0.0.1");
    printf("%d\n", antelope.sin_addr.s_addr);
}

