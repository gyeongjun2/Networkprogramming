#include <stdio.h>
#include <netinet/in.h>

void main(){

	union{
		short s;
		char c[sizeof(short)];

	}sh;

	union{

		long l;
		char c[sizeof(long)];

	}lng;

	sh.s = 0x0102;
	lng.l = 0x01020304;

	printf("=== before ===\n");
	printf("%04x => %02x %02x\n", sh.s, sh.c[0], sh.c[1]);
	printf("%08x => %02x %02x %02x %02x\n", lng.l, lng.c[0], lng.c[1], lng.c[2], lng.c[3]);

	printf("=== after ===\n");
	sh.s = htons(sh.s);
	lng.l = htonl(lng.l);
	printf("%04x => %02x %02x\n", sh.s, sh.c[0], sh.c[1]);
	printf("%08x => %02x %02x %02x %02x\n", lng.l, lng.c[0], lng.c[1], lng.c[2], lng.c[3]);

}
