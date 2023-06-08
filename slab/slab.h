#pragma once

#include <stdint.h>
#include "bitmap.h"

typedef struct slab {
	void*		buffer;
	bitmap		bitmap;
	uint32_t	slab_elem;
	uint8_t		slab_size;
} slab;

void	slabInit(slab* this, void* buffer,bitmap bitmap, uint32_t buffersize, uint8_t slabsize);
void*	slabAlloc(slab* this);
uint8_t	slabFree(slab* this, void* ptr);
