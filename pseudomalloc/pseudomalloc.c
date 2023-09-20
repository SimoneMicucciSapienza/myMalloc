#include "buddy.h"
#include "slab.h"
#include "mhelper.h"

/* _VERBOSE_ as debug for preprocessor*/

//	function called by default before main
void	__attribute__ ((constructor)) pseudoCreate(){
	mhelper_create(helper.mhelper);		//generate memory helper
	buddy_init(
}

//	function called by default after main
void	__attribute__ ((destructor)) pseudoDestroy(){
}

//	switch between all avaible allocator given to allocate a buffer of given size
void*	pseudoAlloc(size_t size){
	// selector
	if (size > 1024) {
		// call mhelper
	}
	else if (size > 64) {
		// call buddy
	}
	else if (size > 16) {
		// call slab 1
	}
	else {
		// call slab 2
	}
}

//	switch between all avaible allocator given to free given buffer
char	pseudoFree(void* mem){

}
