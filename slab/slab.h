#pragma once

#include <stdint.h>
#include "bitmap.h"

typedef struct slab {
	void*	buffer;
	BitMap	bitmap;
	uint8_t	slab_size;
} slab;

void	slabInit(slab* this, void* buffer,BitMap bitmap, uint32_t buffersize, uint8_t slabsize);
void*	slabAlloc(slab* this);
void	slabFree(slab* this, void* ptr);
