#include "color.h"
#include "slab.h"
#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>

static inline uint32_t _ltob_32(uint32_t a){
	char *swp = (char*)&a, buff;
	buff = swp[0];
	swp[0] = swp[3];
	swp[3] = buff;
	buff = swp[1];
	swp[1] = swp[2];
	swp[2] = buff;
	return a;
}

int main(int argv,char* argc[]){
	BitMap map;
	BitMap_init(&map,128,malloc(4*sizeof(uint32_t)));
	slab slab;
	slabInit(&slab,malloc(128*sizeof(uint32_t)),map,128*sizeof(uint32_t),sizeof(uint32_t));
	printf("%08x\t%08x\n",0x01020304,_ltob_32(0x01020304));
	slabAlloc(&slab);
	slabAlloc(&slab);
	slabAlloc(&slab);
	slabAlloc(&slab);
	slabAlloc(&slab);
	slabAlloc(&slab);
	slabAlloc(&slab);
	slabAlloc(&slab);
	return 0;
}
