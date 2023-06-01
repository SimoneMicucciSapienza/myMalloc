#include "bitmap.h"
#include "slab.h"

#include <stdint.h>
#include <stdio.h>

//little endina to big endian
static inline uint32_t _ltob_32(uint32_t a){
	char *swp = (char*)&a, buff;
	buff = swp[0];
	swp[0] = swp[3];
	swp[3] = buff;
	buff = swp[1];
	swp[1] = swp[2];
	swp[2] = buff;
	uint32_t *x = (uint32_t*)swp;
	return *x;
}

//	helper function to find place
static inline uint32_t _lookup_bitmap(BitMap* bitmap){
	uint32_t target, res=1;
	target = BitMap_getBytes(bitmap,res);
		/*locate the bytes to work with*/
	while(target==(~0x0)){
		res+=32;
		target = BitMap_getBytes(bitmap,res);
	}
		/*locate exact position*/
	target = _ltob_32(target);
	while (target>0x7fffffff){
		target = target << 1;
		res++;
	}
	printf("bit %02d\n\n",res);
	return res;
}

//	pointer are responsibilities of the user, bitmap is user responsability
void	slabInit(slab* this, void* buffer,BitMap bitmap, uint32_t buffersize, uint8_t slabsize){
	this->slab_size=slabsize;
	this->buffer=buffer;
	this->bitmap=bitmap;
}

//	return a slab slot
void*	slabAlloc(slab* this){
	uint32_t pos = _lookup_bitmap(&(this->bitmap));
	printf("%08x\t",BitMap_getBytes(&(this->bitmap), pos));
	BitMap_set(&(this->bitmap), pos, 1);
	printf("%08x\n",BitMap_getBytes(&(this->bitmap), pos));
	return NULL;
}

void	slabFree(slab* this, void* ptr);
