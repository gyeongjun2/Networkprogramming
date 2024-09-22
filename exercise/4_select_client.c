#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define SIZE 1024

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[SIZE];
    int fd;
    fd_set readfds;
    
    if (argc != 3) {
        printf("Usage: %s <server-ip> <filename>\n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket() error");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(PORT);

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect() error");
        close(sock);
        exit(2);
    }

    // 파일 이름을 서버에 전송
    if (write(sock, argv[2], strlen(argv[2])) == -1) {
        perror("write() error - failed to send filename");
        close(sock);
        exit(3);
    }

    // 파일 열기
    fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open() error - failed to open file for writing");
        close(sock);
        exit(4);
    }

    // select()를 사용해 소켓과 표준 입력을 동시에 감시
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);   // 서버 소켓 감시
        FD_SET(STDIN_FILENO, &readfds);  // 표준 입력 감시 (키보드 입력)
        
        int max_sd = sock > STDIN_FILENO ? sock : STDIN_FILENO;

        // select 호출
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        // 서버로부터 데이터 수신
        if (FD_ISSET(sock, &readfds)) {
            int read_len = read(sock, buffer, SIZE);
            if (read_len == 0) {
                printf("Server closed the connection\n");
                break;
            } else if (read_len > 0) {
                write(fd, buffer, read_len);  // 파일에 데이터 기록
            } else {
                perror("read() error - failed to read from server");
                break;
            }
        }

        // 사용자의 입력을 처리 (필요한 경우)
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            fgets(buffer, SIZE, stdin);
            if (strncmp(buffer, "exit", 4) == 0) {
                printf("Exiting...\n");
                break;
            }
        }
    }

    printf("File received and saved as %s\n", argv[2]);

    close(fd);
    close(sock);
    return 0;
}
