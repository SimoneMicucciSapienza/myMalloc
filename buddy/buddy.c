#include "buddy.h"

int	MemoryInit(buddy* this, void* pool, void* bitmap_buffer){
	if (!this||!pool||!bitmap_buffer)	return -1;	/*test valid input*/
	this->pool = pool;
	bitmap_init(&(this->bitmap), 1024*(2^5-1), bitmap_buffer);
	return 0;
}

void*	AllocBuddy(buddy* this, uint32_t size);

void	FreeBuddy(buddy* this, void* memory);
