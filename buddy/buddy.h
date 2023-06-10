#pragma once

#include "bitmap.h"

// definition meaning word
#ifndef NULL
#define NULL ((void*)0)
#endif
#define MEMFAIL ((void*) -1)
#define SUCCESS	0
#define FAILURE	-1

/* useful macro for default buddy*/
#define	BITMAP_BIAS	1024
#define	MAX_LEVEL	14
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

int	buddy_init(buddy* this, void* pool, void* bitmap_buffer);
void*	buddy_alloc(buddy* this, uint32_t size);
void	buddy_free(buddy* this, void* memory);
