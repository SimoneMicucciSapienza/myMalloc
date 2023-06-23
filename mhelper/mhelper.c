#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "mhelper.h"
#include "color.h"

#define map(size)\
	(mmap(NULL,size,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS,-1,0))

/*	static inline function helper	*/
/*	custom quick memset (aovid cstd)	*/
static inline void _memset_(void* target, uint64_t size, char value) {
	char* tmp = (char*) target;
	while (size--){
		*tmp++ = value;
	}
}
/*	helper to add elem to linked list	*/
static inline void _set_last_spot(mhelper* this, void* addr, uint64_t size){
	uint64_t rel_size = this->size % 255;
	/*	end of page case	*/
	if (rel_size == 254){
		this->tail->next = map(sizeof(page));	/*	set next on old page		*/
		if (MAP_FAILED==(this->tail->next)){
			dprintf(STDERR_FILENO,"%sERROR%s map failed exception\n",RED,RESET);
			raise(SIGKILL);
		}
		_memset_(this->tail->next, sizeof(page), 0);
		this->tail->next->prev = this->tail;	/*	set previous on new page	*/
		#if _VERBOSE_
		mhelper_print(this);
		#endif
		this->tail = this->tail->next;
	}
	/*	set elem and adjust value	*/
	mem t = {addr,size};
	(this->tail->allocation[rel_size]) = t;
	this->size++;
	#if _VERBOSE_
	printf("(page: %s%p%s displacement: %s%4ld%s)\n",CYAN,this->tail,RESET,GREEN,rel_size,RESET);
	#endif
}
/*	helper to pop (and remove) elem from linked list	*/
static inline mem _get_last_spot(mhelper* this){
	/*	safe null elem case	*/
	mem zero = {NULL,0};
	if (!this->size)
		return zero;
	/*	start accounting to get last page	*/
	this->size--;
	mem res = this->tail->allocation[(this->size)%255];
	/*	default case scenario	*/
	if ((this->size)%255){
		this->tail->allocation[(this->size)%255] = zero;
#if _VERBOSE_
		printf("\t%slast:%s%p%s\tsize:%s%ld%s\n",YELLOW,CYAN,res.address,YELLOW,AZURE,res.size,RESET);
#endif
		return res;
	}
	/*	change page scenario	*/
	void* to_free = (void*)(this->tail);	/*	save current page	*/
	if (!(this->tail->prev))
		return res;	/*	if tail==head avoid removing	*/
	this->tail = this->tail->prev;	/*	update tail	*/
#if _VERBOSE_
	printf("\t(tail:%p\ttarget:%p)\n",this->tail,to_free);
#endif
	munmap(to_free,4096);	/*	free page	*/
#if _VERBOSE_
	mhelper_print(this);
	printf("\t%slast:%s%p%s\tsize:%s%ld%s\n",YELLOW,CYAN,res.address,YELLOW,AZURE,res.size,RESET);
#endif
	return res;
}
/*	helper to find researched page	
 *	ATTENTION! mem will be used not as created but to pass address and displacement!	*/
static inline mem _find_page_elem(mhelper* this, void* search){
	page* curr_page = this->head;
	while (1){
		for(uint8_t i=0;i<255;i++){
			if ((curr_page->allocation[i]).address==search){
				mem ret = {curr_page,i};
				return ret;
			}
		}
		if (curr_page == this->tail)	break;
		curr_page = curr_page->next;
	}
	mem ret = {NULL, 0};
	return ret;
}
/*	unmap page and rewrite value	*/
static inline uint8_t _free_and_substitute(page* page, uint8_t displacement, mem substitute){
	mem temp = page->allocation[displacement];
	uint8_t res = munmap(temp.address,temp.size);
	page->allocation[displacement] = substitute;
	return res;
}

void	mhelper_create(mhelper* this){
#if _VERBOSE_
	printf("\\------------\\\n\033[4;6m%sCREATE HELPER%s\n\\------------\\\n\n",MAGENTA,RESET);
#endif
	if (!this)	return;
	page* page0 = map(sizeof(page));
	if (page0 == MAP_FAILED)	return;
	this->head=this->tail=page0;
	_memset_(this->head,sizeof(page),0);
	this->size=0;
}
void	mhelper_destroy(mhelper* this){
	/*	make routine to unmap all memory on list	*/
#if _VERBOSE_
	printf("\\------------\\\n\033[4;6m%sDESTROY HELPER%s\n\\------------\\\n\n",MAGENTA,RESET);
#endif
	while(this->size){
		mem res = _get_last_spot(this);
printf("\033[4m%sWUH%s\n",YELLOW,RESET);
		if (munmap(res.address,res.size)){
			printf("address: %p\tsize: %ld\n",res.address,res.size);
			dprintf(STDERR_FILENO,"%sERROR%s unmap exception\n",RED,RESET);
			raise(SIGKILL);
		}
		#if _VERBOSE_
		mhelper_print(this);
		#endif
	}
	munmap(this->head,4096);
	this->head = this->tail = NULL;
}
void*	mhelper_alloc(mhelper* this, uint64_t size){
	void* target = map(size);
	if (MAP_FAILED==target){
		dprintf(STDERR_FILENO,"%sERROR%s map failed exception\n",RED,RESET);
		raise(SIGKILL);
	}
	#if _VERBOSE_
	printf("\tallocate: %s%p%s ->\t",ORANGE,target,RESET);
	#endif
	_set_last_spot(this,target,size);
	return target;
}
uint8_t	mhelper_free(mhelper* this, void* memory){
	mem ret = _find_page_elem(this,memory);
	if (!(ret.address))	return -1;
	if (ret.address == this->tail && ret.size == (this->size%255)){
		mem target = _get_last_spot(this);
#if _VERBOSE_
		printf("\t%s(size :%4ld)%s\n",GREEN,this->size,RESET);
#endif
		return munmap(target.address,target.size);
	} else {
		uint8_t res = _free_and_substitute(ret.address,ret.size,_get_last_spot(this));
		return res;
	}
}
void	mhelper_print(mhelper* this){
	printf("\n\\----------------print----------------\\\n");
	printf("\thandler-> %s%p%s\n\t",this?ORANGE:RED,this,RESET);
	if (!this)	return;
	printf("head: %s%p%s tail: %s%p%s size:%s%4ld%s\n",YELLOW,this->head,RESET,YELLOW,this->tail,RESET,GREEN,this->size,RESET);
	page* iterator = this->head;
	printf("\t%s%p%s <= (head -->) %s%p%s <=> ",RED,(void*)(0),RESET,iterator?ORANGE:RED,iterator,RESET);
	while(iterator){
		printf("%s%p%s",iterator->next?ORANGE:RED,iterator->next,RESET);
		if (iterator->next){
			if (iterator->next->next)
				printf(" <=> ");
			else	printf(" (<-- tail) => ");
		}
		iterator = iterator->next;
	}
	printf("\n\\-----------------end-------------------\\");
	printf("\n\n");
}
