#include "bitmap.h"
#include "slab.h"
#include "color.h"

#include <stdint.h>
#include <stdio.h>

//	little endina to big endian
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
static inline uint32_t _lookup_bitmap(bitmap* bitmap){
	uint32_t target, res=0;
	target = bitmap_getBytes(bitmap,res);
		/*locate the bytes to work with*/
	while(target==(~0x0)){
		res+=32;
		target = bitmap_getBytes(bitmap,res);
	}
		/*locate exact position*/
	target = _ltob_32(target);
	while (target>0x7fffffff){
		target = target << 1;
		res++;
	}
	return res;
}

//	pointer are responsibilities of the user, bitmap is user responsability
void	slabInit(slab* this, void* buffer,bitmap bitmap, uint32_t buffersize, uint8_t slabsize){
	this->slab_size=slabsize;
	this->buffer=buffer;
	this->bitmap=bitmap;
	this->slab_elem=this->bitmap.num_bits+1;
}

//	return a slab slot
void*	slabAlloc(slab* this){
	uint32_t pos = _lookup_bitmap(&(this->bitmap));
#ifdef	_LIST_DEBUG_
	printf("bit %s%02d%s :\t%s%08x%s -> ",AZURE,pos,RESET,RED,bitmap_getBytes(&(this->bitmap), pos),RESET);
#endif
	bitmap_set(&(this->bitmap), pos, 1);
#ifdef	_LIST_DEBUG_
	printf("%s%08x%s\t",CYAN,bitmap_getBytes(&(this->bitmap), pos),RESET);
#endif
	void* helper = this->buffer;
	helper += this->slab_size*pos;
	return helper;
}

uint8_t	slabFree(slab* this, void* ptr){
	void *lowerbound, *upperbound;	/*set limits to check validity*/
	lowerbound = this->buffer;
	upperbound = lowerbound + (this->slab_size * this->slab_elem);
	if (lowerbound > ptr || upperbound < ptr){
#ifdef	_LIST_DEBUG_
	printf("slab avaible limit: %s%p%s -> %s%p%s\n",ORANGE,lowerbound,RESET,ORANGE,upperbound,RESET);
#endif
		return -1;
	}
	uint32_t displacement = (uint32_t)(ptr - lowerbound)/this->slab_size;
#ifdef	_LIST_DEBUG_
	printf("lower: %s%p%s\tptr: %s%p%s pos: %s%2d%s\t",
			PURPLE,lowerbound,RESET,
			ORANGE,ptr,RESET,
			AZURE,displacement,RESET);
#endif
	bitmap_set(&(this->bitmap), displacement, 0);
#ifdef	_LIST_DEBUG_
	printf("bitmap: %s%08x%s\t",CYAN,bitmap_getBytes(&(this->bitmap), displacement),RESET);
#endif
	return 0;
}
