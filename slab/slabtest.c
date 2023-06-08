#include "color.h"
#include "slab.h"
#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argv,char* argc[]){
	bitmap map;
	bitmap_init(&map,128,malloc(4*sizeof(uint32_t)));
	slab slab;
	slabInit(
			&slab,
			malloc(128*sizeof(uint32_t)),
			map,
			128*sizeof(uint32_t),
			sizeof(uint32_t));

	printf("\033[2Jallocation test:\n\n"); /*jedy mind trick to clear screen*/
	void* ptr[32];
	for (uint8_t i = 0; i<32; i++) {
		ptr[i] = slabAlloc(&slab);
		printf("%s%p%s\n",ORANGE,ptr[i],RESET);
	}

	printf("deallocation test:\n\n");
	for (int8_t i=0; i<32; i++) {
		uint8_t res=slabFree(&slab, ptr[31-i]);
		printf("iteration %s%2d%s, return %s%2d%s\n",YELLOW,i,RESET,YELLOW,res,RESET);
	}
	return 0;
}
