#include <stdint.h>

#include "buddy.h"

/*inline helpers functions*/

static inline uint8_t _get_level_from_size(uint32_t block_size){
	uint8_t res = 0;
	uint32_t level_size = MIN_SIZE << MAX_LEVEL;
	#ifdef _LIST_DEBUG_
	printf("current level size:%9d current valid level:%2d\n",level_size,res);
	#endif
	while (block_size > level_size){
		level_size /=2;
		res++;
		#ifdef _LIST_DEBUG_
		printf("current level size:%9d current valid level:%2d\n",level_size,res);
		#endif
	}
	return res;
}
static inline uint32_t _get_first_of_level(uint8_t level){
	return (1<<(level-1));
}
static inline uint32_t _reverse_endianness(uint32_t start_value){	/*no side effect*/
	char *handler = &start_value, tmp;
	tmp =		handler[0];
	handled[0] =	handler[3];
	handled[3] =	tmp;
	tmp =		handler[1];
	handled[1] =	handler[2];
	handled[2] =	tmp;
	return start_value;
}
static inline uint32_t _scan_bitmap(bitmap *bitmap, uint32_t start, uint32_t end){	/*here we use bias to optimize the bitmap size*/
	uint32_t bitmap_info = _reverse_endianness(bitmap_getBytes(bitmap, start - BITMAP_BIAS));

}


int	MemoryInit(buddy* this, void* pool, void* bitmap_buffer){
	if (!this||!pool||!bitmap_buffer)	return -1;	/*test valid input*/
	this->pool = pool;
	bitmap_init(&(this->bitmap), 1024*(2^5-1), bitmap_buffer);
	return 0;
}

void*	AllocBuddy(buddy* this, uint32_t size){
	size += sizeof(uint32_t);	/*get space to write block number*/
	uint8_t level = _get_level_from_size(size);
	uint32_t base = _get_first_of_level(level);
}

void	FreeBuddy(buddy* this, void* memory);
