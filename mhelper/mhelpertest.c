#include <stdio.h>
#include <sys/mman.h>

#include "mhelper.h"
#include "color.h"

#define	TEST	500
#define	P_SIZE	2048

#ifndef	_VERBOSE_
#define	_VERBOSE_	1
#endif

int main (int argc, char* argv[]) {
	printf("\033[2J\033[H");
	mhelper base,*this=&base;
	mhelper_create(this);
	void* addresses[TEST];
	mhelper_print(this);
	for (int i=0;i<TEST;i++){
		printf("\033[1;4m%s%03d allocation%s : allocated %5d bytes\n",AZURE,i,RESET,P_SIZE);
		addresses[i]=mhelper_alloc(this,P_SIZE);
	}
	mhelper_print(this);
	for (int i=0;i<TEST;i++){
		printf("\033[1;4m%s%03d deallocation%s : %p\n",AZURE,i,RESET,addresses[i]);
		mhelper_free(this,addresses[i]);
	}
	mhelper_print(this);
	mhelper_destroy(this);
	mhelper_print(this);
	return 0;
}
