#include "chatdr_srv.h"
extern struct cli_info client[];
extern int cnum;

void recv_mesg(int num){

	int len;
	char mesg[MAXMSG], realmsg[MAXMSG];

	memset(mesg, '\0', sizeof(mesg));
	memset(realmsg, '\0', sizeof(realmsg));

	if((len = recv(client[num].clisock, mesg, MAXMSG, 0))<0){
		errmsg("server : message read error");
	}

	if(len == 0)
		return;
	if(!strncmp(mesg, "quit", 4)){
		sprintf(realmsg, "=============\"%s\" Out !! :(\0", client[num].alias);
		printf("%s Disconnect\n", client[num].alias);
		allclisend(num, realmsg);
		client[num] = client[--cnum];
	}else{
		sprintf(realmsg,"[ %-10s ] : ", client[num].alias);
		mesg[len] = '\0';
		strcat(realmsg, mesg);
		allclisend(num, realmsg);
	}
}	
	void allclisend(int num, char *message){
		int i, len;
		len = strlen(message);
		message[len] = '\0';

		for(i = 0;i<cnum;i++){
			if(i==num) continue;
			send(client[i].clisock, message, len, 0);
		}
	}


