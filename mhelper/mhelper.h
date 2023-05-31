#pragma once

#include <stdint.h>

#define	MEM_ERR		((void*)-1)
#define	MEM_FREE	((void*)0)

//	page representation for munmap
typedef	struct {
	void*		addr;
	uint64_t	bytes;
} page;
//	redefine the page pointer to identify arrays
typedef	page*	pagePtr;
//	memory listhead
typedef struct {
	pagePtr		first;
	pagePtr		last;
	uint64_t	size;
} mhelper;

//	initialize 4 page for the bookkeeper space
void	mhelper_create(mhelper* this);
//	free the pending page opened and at last the current page of holder
void	mhelper_destroy(mhelper* this);
//	wrapping for a mmap wt bookkeeping
void*	mhelper_alloc(mhelper* this, uint64_t size);
//	wrapping for a munmap wt bookkeeping
uint8_t	mhelper_free(mhelper* this, void* memory);
