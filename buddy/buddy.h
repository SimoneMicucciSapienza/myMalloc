#pragma once

#include "bitmap.h"

// definition meaning word
#define NULL ((void*)0)
#define MEMFAIL ((void*) -1)
#define SUCCESS	0
#define FAILURE	-1


/* 
 * 1MiB to be allocated
 * fixed deepness
 * granularity from 1024B to 64B
 **/
typedef struct buddy {
	bitmap*	bitmap;
	void* pool;
} buddy;

int	MemoryInit(void* hugepage);
int	MemoryDestroy();
void*	AllocBuddy(unsigned long size);
void	FreeBuddy(void* buddy);
