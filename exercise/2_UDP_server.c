#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024
#define END_OF_FILE "EOF"  // 파일 전송이 끝났음을 알리는 신호

void error_handling(char *message);

int main() {
    int server_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char buffer[SIZE];
    int fd;

    // UDP 소켓 생성
    server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_sock == -1)
        error_handling("socket() error");

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        error_handling("bind() error");

    client_addr_size = sizeof(client_addr);

    // 파일 이름 수신
    int str_len = recvfrom(server_sock, buffer, SIZE - 1, 0, (struct sockaddr*)&client_addr, &client_addr_size);
    if (str_len == -1)
        error_handling("recvfrom() error");
    buffer[str_len] = '\0';  // 파일 이름의 끝에 NULL 문자 추가

    // 파일 열기
    fd = open(buffer, O_RDONLY);
    if (fd == -1) {
        printf("File not found: %s\n", buffer);
        close(server_sock);
        return 1;
    }

    // 파일 데이터를 클라이언트로 전송
    int read_len;
    while ((read_len = read(fd, buffer, SIZE)) > 0) {
        sendto(server_sock, buffer, read_len, 0, (struct sockaddr*)&client_addr, client_addr_size);
    }

    // 파일 전송 완료 후 종료 신호 전송
    sendto(server_sock, END_OF_FILE, strlen(END_OF_FILE), 0, (struct sockaddr*)&client_addr, client_addr_size);

    printf("File transfer complete.\n");

    close(fd);
    close(server_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
