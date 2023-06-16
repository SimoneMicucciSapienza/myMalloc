#pragma once

#include <stdint.h>
#include <sys/mman.h>

#define	MEM_ERR		((void*)-1)
#define	MEM_FREE	((void*)0)

typedef struct mem {
	void*		address;
	uint64_t	size;
} __attribute__((packed)) mem;	/*not required but better have it anyway*/

typedef struct page {
	struct page*	prev;
	struct page*	next;
	mem		allocation[255];
} __attribute__((packed)) page;	/*not required but better have it anyway*/

typedef struct mhelper {
	page*	head;
	page*	tail;
	uint64_t	size;
} __attribute((packed)) mhelper;

void	mhelper_create(mhelper* this);
void	mhelper_destroy(mhelper* this);
void*	mhelper_alloc(mhelper* this, uint64_t size);
uint8_t	mhelper_free(mhelper* this, void* memory);
