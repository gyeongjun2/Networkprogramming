#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define SIZE 1024
#define END_OF_FILE "EOF"  // 파일 전송이 끝났음을 알리는 신호

int main(int argc, char *argv[]) {

    int sock;
    struct sockaddr_in server_addr;
    char buffer[SIZE];
    int fd;

    if (argc != 3) {
        printf("Usage: %s <server-ip> <filename>\n", argv[0]);
        exit(1);
    }

    // UDP 소켓 생성
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("socket() error");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(PORT);

    socklen_t addr_size = sizeof(server_addr);

    // 파일 이름 전송
    sendto(sock, argv[2], strlen(argv[2]), 0, (struct sockaddr *)&server_addr, addr_size);

    // 파일 열기
    fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Failed to open file: %s\n", argv[2]);
        close(sock);
        return 1;
    }

    // recvfrom()에 타임아웃 설정
    struct timeval tv;
    tv.tv_sec = 5;  // 5초 타임아웃
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int read_len;

    // 파일 수신 루프
    while ((read_len = recvfrom(sock, buffer, SIZE, 0, NULL, NULL)) > 0) {
        // 서버가 EOF 신호를 보내면 루프 종료
        if (strncmp(buffer, END_OF_FILE, strlen(END_OF_FILE)) == 0) {
            break;
        }

        // 파일에 쓰기
        write(fd, buffer, read_len);
    }

    if (read_len == -1) {
        printf("recvfrom() timed out or failed.\n");
    } else {
        printf("File received successfully.\n");
    }

    close(fd);
    close(sock);
    return 0;
}
