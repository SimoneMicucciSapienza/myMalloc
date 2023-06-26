#include <stdio.h>
#include <sys/mman.h>

#include "mhelper.h"
#include "color.h"

#define	TEST	1
#define	P_SIZE	2048

#ifndef	_VERBOSE_
#define	_VERBOSE_	1
#endif

int main (int argc, char* argv[]) {
	printf("\033[2J\033[H");
	mhelper base,*this=&base;
	mhelper_create(this);
	void* addresses[TEST];
	for (int i=0;i<TEST;i++){
		printf("\033[1;4m%s%03d allocation%s :",AZURE,i,RESET);
		addresses[i]=mhelper_alloc(this,P_SIZE);
		printf("\n");
		mhelper_print(this);
	}
	for (int i=TEST-1;i>-1;i--){
		printf("\033[1;4m%s%03d deallocation%s :",AZURE,i,RESET);
		mhelper_free(this,addresses[i]);
		printf("\n");
		mhelper_print(this);
	}
	mhelper_destroy(this);
	return 0;
}
