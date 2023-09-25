#include "buddy.h"
#include "slab.h"
#include "mhelper.h"

/* _VERBOSE_ as debug for preprocessor*/

//	function called by default before main
void	__attribute__ ((constructor)) pseudoCreate(){
	// generate memory helper
	mhelper_create(&(helper.mhelper));
	// initialize buddy with memory helper
	buddy_init(	&(helper.buddy),
			mhelper_alloc(&(helper.mhelper),(1024*1024)),
			mhelper_alloc(&(helper.mhelper),(31*128));
	// initialize big slab with memory helper
	slabInit(	&(helper.big_slab),
			mhelper_alloc(&(helper.mhelper),(64*1024)),
			mhelper_alloc(&(helper.mhelper),(128)),
			1024,
			64);
	// initialize small slab with memory helper
	slabInit(	&(helper.small_slab),
			mhelper_alloc(&(helper.mhelper),(16*1024)),
			mhelper_alloc(&(helper.mhelper),(128)),
			1024,
			16);
}

//	function called by default after main
void	__attribute__ ((destructor)) pseudoDestroy(){
	mhelper_destroy(&(helper.mhelper));
}

//	switch between all avaible allocator given to allocate a buffer of given size
void*	pseudoAlloc(size_t size){
	// selector
	if (size > 1016) {	//buddy require space to store the size
		// call mhelper
		return mhelper_alloc(&(helper.mhelper),size);
	}
	else if (size > 64) {
		// call buddy
		return buddy_alloc(&(helper.buddy),size);
	}
	else if (size > 16) {
		// call big slab
		return slabAllocator(&(helper.big_slab));
	}
	else {
		// call small slab
		return slabAllocator(&(helper.small_slab));
	}
}

//	switch between all avaible allocator given to free given buffer
char	pseudoFree(void* mem){
	if (mem >= helper.buddy.pool && mem <= (helper.buddy.pool + 1024*1024))
		buddy_free(helper.buddy, mem);
	else if (mem >= helper.big_slab.buffer && mem <= (helper.big_slab.buffer + helper.big_slab.slab_elem * helper.big_slab.slab_size))
		slabFree(helper.big_slab, mem);
	else if (mem >= helper.small_slab.buffer && mem <= (helper.small_slab.buffer + helper.small_slab.slab_elem * helper.small_slab.slab_size))
		slabFree(helper.small_slab, mem);
	else
		mhelper_free(helper.mhelper, mem);
	return 0;
}
