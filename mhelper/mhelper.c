#include "mhelper.h"
#include <sys/mman.h>

#define	P_FLAG	PROT_READ|PROT_WRITE
#define	M_FLAG	MAP_PRIVATE|MAP_ANONYMOUS

//	initialize std ll elem
static inline void** _page_init(void){
	void** res = (void**)mmap(0,4096,P_FLAG,M_FLAG,-1,0);	/*init var & mmap std page*/
	if (res==MEM_FAILED)	return res;	/*check mmap*/
	for (int i=0;i<(4096/sizeof(void*));i++)	/*res as void res[], loop all to set from 0 to 1023*/
		res[i]=MEM_FREE;	/*set all to free memory*/
	return res;	/*initialized page*/
}

//	given size return exact position to write
static inline void** _scan_list(void** head, uint64_t pos){
	if (!head)	/*better safe than sorry*/
		return MEM_ERR;	/*default error code*/
	void** current_page = head;	/*set current page*/
	uint64_t relative_position= pos;	/*set relative position*/
	while (relative_position > (4096/sizeof(void*))){	/*untill we find right page*/
		if (!current_page)	/*check wrong position*/
			return MEM_ERR;	/*default error code*/
		relative_position -= (4096/sizeof(void*));	/*remove page size position from current pos*/
		current_page = (void**)(current_page[(4096/sizeof(void*))]);	/*switch as next page the current*/
	}
	return (void**)(&current_page[relative_size]);	/*return pointer to position*/
}

void	mhelper_create(mhelper* this){
	if (!this)
		return;
	this->size = 0;
	this->bookkeeper = (void**)_page_init();
}

void	mhelper_destroy(mhelper* this){
}

void*	mhelper_alloc(mhelper* this, uint64_t size){
	void* res = MEM_FREE;
	if (!this)
		return res;
	if (!(this->bookkeeper))
		return res;
	res = mmap(0,size, P_FLAG, M_FLAG, -1, 0);
	if (res==MEM_FREE)
		return res;
	uint64_t r_size = this->size;
	if (res > 1024)

}

uint8_t	mhelper_free(mhelper* this, void* memory){
}
