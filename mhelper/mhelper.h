#pragma once

#include <stdint.h>

#define	MEM_FREE	((void*)-1)

/*
 * bookkeeping for memory mapping
 * bookkeeper is a page with doubling space and copy on demand
 * in short is a big "void*" array allocated with a mmap
 *
 * this consent to keep track of wrong address to free and to destroy stuff after end of process
 *
 * */
typedef struct {
	void**	bookkeeper;
} mhelper;

mhelper	handler

//	initialize 4 page for the bookkeeper space
void	mhelper_create(void);
//	free the pending page opened and at last the current page of holder
void	mhelper_destroy(mhelper* this);
//	wrapping for a mmap wt bookkeeping
void*	mhelper_alloc(uint64_t size);
//	wrapping for a munmap wt bookkeeping
uint8_t	mhelper_free(void* memory);
