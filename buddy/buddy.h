#pragma once

// definition meaning word
#define NULL ((void*)0)
#define MEMFAIL ((void*) -1)
#define SUCCESS	0
#define FAILURE	-1


/* 
 * 1MiB to be allocated
 * granularity of 64B
 * deepness of 14 level
 **/
typedef struct buddy {
	void* bitmap;
	void* pool;
} buddy;

int	MemoryInit();
int	MemoryDestroy();
void*	AllocBuddy(unsigned long size);
void	FreeBuddy(void* buddy);
