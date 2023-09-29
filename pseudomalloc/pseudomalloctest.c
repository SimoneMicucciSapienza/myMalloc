#include "pseudomalloc.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	printf("-------------------main--------------------\n");
	char *block1, *block2, *block3, *block4;
	block1 = pseudoAlloc(10);
	printf("  10B allocation = %p\n\n",block1);
	block2 = pseudoAlloc(50);
	printf("  50B allocation = %p\n\n",block2);
	block3 = pseudoAlloc(500);
	printf(" 500B allocation = %p\n\n",block3);
	block4 = pseudoAlloc(2000);
	printf("2000B allocation = %p\n\n",block4);
	printf("-------------------------------------------\n");
	return 0;
}
