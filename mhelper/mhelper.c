#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "mhelper.h"
#include "color.h"

#define map(size)\
	(mmap(NULL,size,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS,-1,0))

static inline void memset_(void* target, uint64_t size, char value) {
	char* tmp = (char*) target;
	while (size--){
		*tmp++ = value;
	}
}
static inline void set_last_spot(mhelper* this, void* addr, uint64_t size){
	uint64_t rel_size = this->size % 255;
	page* rel_page = this->tail;
	/*	end of page case	*/
	if (rel_size == 254){
		rel_page->next = map(sizeof(page));
		if (MAP_FAILED==(rel_page->next)){
			dprintf(STDERR_FILENO,"%sERROR%s map failed exception\n",RED,RESET);
			raise(SIGKILL);
		}
		this->tail = this->tail->next;
		memset_(this->tail, sizeof(page), 0);
	}
	/*	set elem and adjust value	*/
	mem t = {addr,size};
	(rel_page->allocation[rel_size]) = t;
	this->size++;
}

void	mhelper_create(mhelper* this){
	if (!this)	return;
	page* page0 = map(sizeof(page));
	if (page0 == MAP_FAILED)	return;
	this->head=this->tail=page0;
	memset_(this->head,sizeof(page),0);
}
void	mhelper_destroy(mhelper* this){
	/*	make routine to unmap all memory on list	*/
}
void*	mhelper_alloc(mhelper* this, uint64_t size){
	void* target = map(size);
	if (MAP_FAILED==target){
		dprintf(STDERR_FILENO,"%sERROR%s map failed exception\n",RED,RESET);
		raise(SIGKILL);
	}
	set_last_spot(this,target,size);
	return target;
}
uint8_t	mhelper_free(mhelper* this, void* memory){}
