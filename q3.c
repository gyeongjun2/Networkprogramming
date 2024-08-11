#include <string.h>
#include <stdio.h>

void main(){

	int temp, rtn;
	int val=66;

	memset(&temp, 5, sizeof(temp));
	printf("%08x\n", temp);

	memset(&temp, 0, sizeof(temp));
	printf("%d\n", temp);

	memcpy(&temp, &val, sizeof(int));
	printf("%d\n", temp);

	printf("rtn= %d\n", rtn=memcmp(&temp, "A", 1));
	printf("rtn= %d\n", rtn=memcmp(&temp, "B", 1));
	printf("rtn= %d\n", rtn=memcmp(&temp, "C", 1));

}
