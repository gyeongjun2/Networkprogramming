#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>

#define PORT 8080
#define SIZE 1024
#define MAX_CLIENTS 30

int main() {

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    int fd;
    char buffer[SIZE];
    int i;


    int client_socks[MAX_CLIENTS];
    int max_sd, activity, sd, new_socket;
    fd_set readfds;
    

    for (i = 0; i < MAX_CLIENTS; i++) {
        client_socks[i] = -1;
    }


    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("socket() error");
        exit(1);
    }


    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

  
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind() error");
        close(server_sock);
        exit(2);
    }

    
    if (listen(server_sock, 5) == -1) {
        perror("listen() error");
        close(server_sock);
        exit(3);
    }

    client_addr_size = sizeof(client_addr);

    while (1) {
       
        FD_ZERO(&readfds);
        FD_SET(server_sock, &readfds);
        max_sd = server_sock;

        
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socks[i];

            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }

       
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);


        
        if (FD_ISSET(server_sock, &readfds)) {
            new_socket = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
            if (new_socket < 0) {
                perror("accept() error");
                exit(4);
            }

            
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_socks[i] == -1) {
                    client_socks[i] = new_socket;
                    printf("Added new client to list: [%d]\n", i);
                    break;
                }
            }

            if (i == MAX_CLIENTS) {
                printf("Max clients reached. Cannot accept new clients.\n");
                close(new_socket);
            }
        }

        
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_socks[i];

            if (FD_ISSET(sd, &readfds)) {
                
                int str_len = read(sd, buffer, SIZE - 1);
                if (str_len == 0) {
                    
                    printf("Client disconnected: [%d]\n", i);
                    close(sd);
                    client_socks[i] = -1;
                } else {
                    buffer[str_len] = '\0';  

                   
                    fd = open(buffer, O_RDONLY);
                    if (fd == -1) {
                        perror("File not found");
                        close(sd);
                        client_socks[i] = -1;
                    } else {
                        
                        while (1) {
                            int read_len = read(fd, buffer, SIZE);
                            if (read_len == 0) break;
                            write(sd, buffer, read_len);
                        }
                        printf("File transfer to client [%d] complete\n", i);
                        close(fd);
                        close(sd);
                        client_socks[i] = -1;
                    }
                }
            }
        }
    }

    // 소켓 닫기
    close(server_sock);
    return 0;
}
