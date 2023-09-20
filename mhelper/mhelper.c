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
/*	make a quick handler page	*/
static inline page* _getpage_(void){
	page* res = (page*)map(4096);
	if (res==MAP_FAILED){
		dprintf(STDERR_FILENO,"%sERROR%s memory mapping failed\t",RED,RESET);
		raise(SIGILL);
	}
	//_memset_(res,4096,0x00);
	return res;
}
/*	set last memory elemeny in handler	*/
static inline void _memory_push(mhelper* this,mem value){
	//write in next page
	if (!(this->size%255) && this->size){
		if (!this->tail->next)	{	this->tail->next = _getpage_();	}
		this->tail->next->prev = this->tail;
		this->tail = this->tail->next;
	}
	//set value
	this->tail->allocation[this->size%255] = value;
	//adjust handler structure
	this->size++;
}
/*	get last memory elemeny from handler	*/
static inline mem _memory_pop(mhelper* this, mem zero){
	//point to last allocated item
	this->size--;
	//adjust tail page pointer
	if (!(this->size%255) && this->size){
		this->tail = this->tail->prev;
	}
	//to be safe
	if (!this->tail){
		dprintf(STDERR_FILENO,"%sERROR%s structure linking failure\t",RED,RESET);
		raise(SIGILL);
	}
	printf("%s(size:%3ld->%14p-%ld)%s",GRAY,this->size,this->tail->allocation[this->size%255].address,this->tail->allocation[this->size%255].size,RESET);
	//minimal declaration
	mem res;
	//saving value
	res = this->tail->allocation[this->size%255];
	//reset handler
	this->tail->allocation[this->size%255] = zero;
	//return requested value
	printf("%s(size:%3ld->%14p-%ld)%s",GRAY,this->size,this->tail->allocation[this->size%255].address,this->tail->allocation[this->size%255].size,RESET);
	return res;
}

void	mhelper_create(mhelper* this){
	this->size=0;
	this->head=this->tail=_getpage_();
	#if _VERBOSE_
		printf("\n\033[4;5m%sCREATE MEMORY HELPER%s\n\n",MAGENTA,RESET);
		mhelper_print(this);
	#endif
}
void	mhelper_destroy(mhelper* this){
	//free every allocated block
	while (this->size){
		mem zero = {NULL,0};
		mem target = _memory_pop(this,zero);
		munmap(target.address,target.size);
	}
	//free every handler page
	page* iter = this->head;
	while (iter!=NULL){
		page* temp = iter;
		iter = iter->next;
		munmap(temp,4096);
	}
	//set real value to structure's pointer
	this->head=this->tail=NULL;
	#if _VERBOSE_
		printf("\n\033[4;5m%sDESTROY MEMORY HELPER%s\n\n",MAGENTA,RESET);
		mhelper_print(this);
	#endif
	//this->size=this->available=0;
}
void*	mhelper_alloc(mhelper* this, uint64_t size){
	//initialize value and update structure
	mem value = {map(size),size};
	_memory_push(this,value);
	if (value.address==MAP_FAILED){
		dprintf(STDERR_FILENO,"%sERROR%s memory mapping failed\t",RED,RESET);
		raise(SIGILL);
	}
	#if _VERBOSE_
		printf("\t(%s%p%s,%s%8ld%s)",value.address?ORANGE:RED,value.address,RESET,YELLOW,value.size,RESET);
	#endif
	return value.address;
}
uint8_t	mhelper_free(mhelper* this, void* memory){
	mem target = {NULL,0}, swap = _memory_pop(this,target);
	volatile page *iter = this->head;
	uint8_t relsize = 0,npage=0;
	//find target memory
	while (iter->allocation[relsize].address){
		if ((iter->allocation[relsize]).address==memory){
			target = iter->allocation[relsize];
			break;
		}
		relsize++;
		if (relsize==255){
			iter = iter->next;
			npage++;
			relsize = 0;
		}
	}
	#if _VERBOSE_
		printf("\t(%s%p%s(%s%3d%s,%s%3d%s))",
				target.address?ORANGE:RED,target.address,RESET,
				GREEN,npage,RESET,
				MAGENTA,relsize,RESET);
	#endif
	//free target
	uint8_t res = munmap(target.address,target.size);
	//substitute void value wt last only if not last spot
	if (relsize != this->size%255 || iter != this->tail)
		iter->allocation[relsize]=swap;
	return res;
}
void	mhelper_print(mhelper* this){
	printf("\thandler: %s%p%s\n",this?ORANGE:RED,this,RESET);
	if (!this)	return;
	printf("\t\tsize: %s%ld%s\n",AZURE,this->size,RESET);
	volatile page* iterator = this->head;
	printf("\t\t");
	while (iterator!=NULL){
		if (iterator==this->head)	printf("(%shead%s)-> ",YELLOW,RESET);
		printf("%s%p%s",iterator!=NULL?ORANGE:RED,iterator,RESET);
		if (iterator==this->tail)	printf(" <-(%stail%s)",YELLOW,RESET);
		if (iterator->next!=NULL)
			printf(" <-> ");
		iterator = iterator->next;
	}
	printf("\n\n");
	#if _VERBOSE_
		iterator = this->head;
		uint8_t pos = 0;
		while(1){
			if (!pos)	printf("\n\t%s%p%s",YELLOW,iterator,RESET);
			if (!iterator)	break;
			if (!(pos%8))	printf("\n\t\t");
			printf("%s%18p%s-%6ld ",(iterator->allocation[pos].address)?RESET:PINK,iterator->allocation[pos].address,RESET,iterator->allocation[pos].size);
			pos++;
			if (pos==255) {
				pos=0;
				iterator = iterator->next;
			}
		}
		printf("\n\n");
	#endif
	return;
}
