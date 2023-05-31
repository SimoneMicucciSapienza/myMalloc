#include "mhelper.h"
#include <sys/mman.h>

#define	P_FLAG	PROT_READ|PROT_WRITE
#define	M_FLAG	MAP_PRIVATE|MAP_ANONYMOUS
#define	P_SIZE	4096
#define	P_ELEM	P_SIZE/sizeof(page)

//	get page number
static inline uint64_t _page_number(mhelper* this){
	return this->size/P_ELEM;
}
//	initialize std ll elem
static inline pagePtr _page_init(void){
	pagePtr res = (pagePtr)mmap(0,4096,P_FLAG,M_FLAG,-1,0);	/*init var & mmap std page*/
	if (res==MAP_FAILED)	return res;	/*check mmap*/
	page tmp = {MEM_FREE,0};	/*free memory elem template*/
	for (int i=0;i<P_ELEM;i++)	/*res as void res[], loop all to set from 0 to 511*/
		res[i]=tmp;	/*set all to free memory*/
	return res;	/*initialized page*/
}

//	given size return exact position to write
static inline pagePtr _scan_list(pagePtr head, uint64_t pos){
	if (!head)	/*better safe than sorry*/
		return MEM_ERR;	/*default error code*/
	pagePtr current_page = head;	/*set current page*/
	uint64_t relative_position= pos;	/*set relative position*/
	while (relative_position > P_ELEM){	/*untill we find right page*/
		if (!current_page)	/*check wrong position*/
			return MEM_ERR;	/*default error code*/
		relative_position -= P_ELEM;	/*remove page size position from current pos*/
		current_page = (pagePtr)((current_page[P_ELEM]).addr);	/*switch as next page the current*/
	}
	return &current_page[relative_position];	/*return pointer to position*/
}

void	mhelper_create(mhelper* this){
	if (!this)
		return;
	this->size = 0;
	this->first = _page_init();
	this->last = this->first;
}

void	mhelper_destroy(mhelper* this){
}

void*	mhelper_alloc(mhelper* this, uint64_t size){
	//shit test
	void* res = MEM_FREE;
	if (!this)
		return res;
	if (!(this->first))
		return res;
	//try alloc
	res = mmap(0,size, P_FLAG, M_FLAG, -1, 0);
	if (res==MEM_FREE)
		return res;
	//book keeping
	this->size++;
	if ((this->size%P_ELEM)==(P_ELEM-1)){						//if we have to add a new book keeping page
		(this->last[P_ELEM-1]).addr = (void*)_page_init();			//set new page addr
		(this->last[P_ELEM-1]).bytes = P_SIZE;					//set new page size
		if ((this->last[P_ELEM-2]).addr==MAP_FAILED)
			return MAP_FAILED;						//just to avoid problem
		pagePtr _new_last = (pagePtr)((this->last[P_ELEM-1]).addr);		//new page reference
		this->size++;								//new book keeping page written
		(_new_last[0]).addr = (void*)(this->last);				//double linked list prev addr
		(_new_last[0]).bytes = P_SIZE;						//double linked list prev size
		this->last = _new_last;							//update helper reference
		this->size++;								//previous book keeping page written
	}
	(this->last[size%P_ELEM]).addr = res;		//mark the pointer
	(this->last[size%P_ELEM]).bytes = size;		//mark the size
	return res;
}
