#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>


#define SIZE 1024


int main(int argc, char *argv[]){

	int server_sock, client_sock;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t cli_addr_size;

	//create socket
	server_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//setting serv_addr
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//bind socket
	bind(server_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	
	//wait connect
	listen(server_sock, 5);

	printf("server is running...[port number : %s]\n",argv[1]);

	while(1){
		cli_addr_size = sizeof(cli_addr);

		//accept client
		client_sock = accept(server_sock, (struct sockaddr*)&cli_addr,&cli_addr_size);

		
		char command[SIZE];
		int read_len;
		read_len = read(client_sock,command,SIZE);
		command[read_len] = '\0';

		printf("start command : %s\n",command);

		int status=0;
		int pid = fork();

		if(pid==0){	//child
			close(1);
			dup(client_sock);
		
			//"/bin/sh"-> 기본 쉘을 의미, "-c" -> 뒤 명령어를 문자열로 처리
			execl("/bin/sh","sh","-c",command,NULL);
			perror("execl() error");
			close(client_sock);
			exit(1);

		}
		//parent
		wait(&status);
		close(client_sock);
		

	}

}
