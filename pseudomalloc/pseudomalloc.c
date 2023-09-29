#include "pseudomalloc.h"
#include "buddy.h"
#include "slab.h"
#include "mhelper.h"
#include "bitmap.h"
#include <stdio.h>

/* _VERBOSE_ as debug for preprocessor*/

//	function called by default before main

/*	creation of helper	*/
allocator_helper	pseudohelper;

void	__attribute__ ((constructor)) pseudoCreate(){
	// generate memory helper
	mhelper_create(&(pseudohelper.mhelper));
	// initialize buddy with memory helper
	buddy_init(	&(pseudohelper.buddy),
			mhelper_alloc(&(pseudohelper.mhelper),(1024*1024)),
			mhelper_alloc(&(pseudohelper.mhelper),(31*128)));
	// initialize big slab with memory helper
	bitmap sample;
	bitmap_init(&sample,128,mhelper_alloc(&(pseudohelper.mhelper),(128))),
	slabInit(	&(pseudohelper.big_slab),
			mhelper_alloc(&(pseudohelper.mhelper),(64*1024)),
			sample,
			1024,
			64);
	// initialize small slab with memory helper
	bitmap_init(&sample,128,mhelper_alloc(&(pseudohelper.mhelper),(128))),
	slabInit(	&(pseudohelper.small_slab),
			mhelper_alloc(&(pseudohelper.mhelper),(16*1024)),
			sample,
			1024,
			16);
	printf("----------------constructor----------------\n");
	printf("buddy: %p ~ %p\nbig slab: %p ~ %p\nsmall slab: %p ~ %p\n",
			pseudohelper.buddy.pool, pseudohelper.buddy.pool + 1024*1024,
			pseudohelper.big_slab.buffer, pseudohelper.big_slab.buffer + 128*64,
			pseudohelper.small_slab.buffer, pseudohelper.small_slab.buffer + 128*16);
	printf("-------------------------------------------\n");
}

//	function called by default after main
void	__attribute__ ((destructor)) pseudoDestroy(){
	printf("----------------destructor-----------------\n");
	mhelper_destroy(&(pseudohelper.mhelper));
	printf("-------------------------------------------\n");
}

//	switch between all avaible allocator given to allocate a buffer of given size
void*	pseudoAlloc(size_t size){
	// selector
	if (size > 1016) {	//buddy require space to store the size
		// call mhelper
		return mhelper_alloc(&(pseudohelper.mhelper),size);
	}
	else if (size > 64) {
		// call buddy
		return buddy_alloc(&(pseudohelper.buddy),size);
	}
	else if (size > 16) {
		// call big slab
		return slabAlloc(&(pseudohelper.big_slab));
	}
	else {
		// call small slab
		return slabAlloc(&(pseudohelper.small_slab));
	}
}

//	switch between all avaible allocator given to free given buffer
char	pseudoFree(void* mem){
	if (mem >= pseudohelper.buddy.pool && mem <= (pseudohelper.buddy.pool + 1024*1024))
		buddy_free(&(pseudohelper.buddy), mem);
	else if (mem >= pseudohelper.big_slab.buffer && mem <= (pseudohelper.big_slab.buffer + pseudohelper.big_slab.slab_elem * pseudohelper.big_slab.slab_size))
		slabFree(&(pseudohelper.big_slab), mem);
	else if (mem >= pseudohelper.small_slab.buffer && mem <= (pseudohelper.small_slab.buffer + pseudohelper.small_slab.slab_elem * pseudohelper.small_slab.slab_size))
		slabFree(&(pseudohelper.small_slab), mem);
	else
		mhelper_free(&(pseudohelper.mhelper), mem);
	return 0;
}
