#pragma once

// definition meaning word
#define NULL ((void*)0)
#define SUCCESS	0
#define FAILURE	-1


/* 
 * 1MiB to be allocated
 * granularity of 1B
 *
 **/
typedef struct buddy {
	void* bitmap;
	void* pool;
} buddy;

int	MemoryInit();
int	MemoryDestroy();
void*	AllocBuddy(unsigned long size);
void	FreeBuddy(void* buddy);
