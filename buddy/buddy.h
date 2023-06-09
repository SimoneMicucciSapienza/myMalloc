#pragma once

#include "bitmap.h"

// definition meaning word
#define NULL ((void*)0)
#define MEMFAIL ((void*) -1)
#define SUCCESS	0
#define FAILURE	-1

/* useful macro for this exact buddy*/
#define	BITMAP_BIAS	1024
#define	MAX_LEVEL	15
#define	MIN_SIZE	64

/* 
 * 1MiB to be allocated
 * fixed deepness
 * granularity from 1024B to 64B
 **/
typedef struct buddy {
	void*	pool;
	bitmap	bitmap;
} buddy;

int	MemoryInit(buddy* this, void* pool, void* bitmap_buffer);
void*	AllocBuddy(buddy* this, uint32_t size);
void	FreeBuddy(buddy* this, void* memory);
