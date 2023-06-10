#if _VERBOSE_
#include <stdio.h>
#include "color.h"
#endif
#include <stdint.h>

#include "buddy.h"

/*
 * inline helpers functions
 * */

/*	from the size return the current level of buddy request*/
static inline uint8_t _get_level_from_size(uint32_t block_size){
	uint8_t res = 0;
	uint32_t level_size = MIN_SIZE << MAX_LEVEL;
	while (block_size < level_size){
		#if _VERBOSE_ 
		printf("\tcurrent level size:%s%9d%s current level:%s%2d%s current block:%s%4d%s\n",
				MAGENTA,level_size,RESET,AZURE,res,RESET,YELLOW,block_size,RESET);
		#endif
		level_size /=2;
		res++;
	}
	level_size = level_size << 1;
	#if _VERBOSE_ 
	printf("\tcurrent level size:%s%9d%s correct level:%s%2d%s current block:%s%4d%s\n",
			ORANGE,level_size,RESET,GREEN,res,RESET,ORANGE,block_size,RESET);
	#endif
	return res;
}
/*	get relative 0 block*/
static inline uint32_t _get_first_of_level(uint8_t level){
	return (1<<(level-1));
}
/*	required for little endian architecture to parse bitmap by word*/
static inline uint32_t _reverse_endianness(uint32_t start_value){	/*no side effect*/
	char *handler = (char*)(&start_value), tmp;
	tmp =		handler[0];
	handler[0] =	handler[3];
	handler[3] =	tmp;
	tmp =		handler[1];
	handler[1] =	handler[2];
	handler[2] =	tmp;
	return start_value;
}
/*	debug print function*/
#if _VERBOSE_
static const uint8_t print_mask[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};	/*	bit mask	*/
static inline void _print_bytes(uint32_t target){
	printf("\t%08x -> ",target);
	char* prt = (char*)(&target);
	printf("\t");
	for (uint8_t i = 0; i<32;i++){
		if (i!=0 && (i%8)==0)	prt++;
		printf("%d",((*prt) & print_mask[i%8])?1:0);
		if (i%4==3)	printf(" ");
		target = target << 1;
	}
	printf("\n");
}
#endif
/*	bitmap parser to reach offset*/
static inline int _get_bitmap_offset(bitmap *bitmap, uint32_t start, uint32_t end){	/*here we use bitmap bias*/
	uint32_t bitmap_chunk = _reverse_endianness(bitmap_getBytes(bitmap, start - BITMAP_BIAS));
	uint32_t offset = 0, max_offset = end - start;
	while (bitmap_chunk == 0xffffffff){
		offset +=32;
		bitmap_chunk = _reverse_endianness(bitmap_getBytes(bitmap, start - BITMAP_BIAS + offset));
		#if _VERBOSE_
		_print_bytes(bitmap_chunk);
		#endif
	}
	while (bitmap_chunk>0x7fffffff && bitmap_chunk){
		bitmap_chunk = bitmap_chunk << 1;
		offset++;
		#if _VERBOSE_
		printf("%s%04d%s:\t",AZURE,bitmap_chunk,RESET);
		_print_bytes(bitmap_chunk);
		#endif
	}
	if (offset > max_offset)
		return FAILURE;
	return offset;
}
/*	set bitmap helper*/
static inline void _set_bitmap_biased(buddy* this, int buddy_num, uint8_t status){
	bitmap_set(&(this->bitmap),buddy_num - BITMAP_BIAS, status);
	#if _VERBOSE_
	printf("%s%04d%s ->\t",MAGENTA,buddy_num - BITMAP_BIAS,RESET);
	_print_bytes(_reverse_endianness(bitmap_getBytes(&(this->bitmap),buddy_num - BITMAP_BIAS)));
	#endif
}
/*	get size of level*/
static inline uint32_t _get_level_block_size(uint8_t level){
	uint32_t bytes = MIN_SIZE;
	while (level++ != MAX_LEVEL)
		bytes = bytes<<1;
	return bytes;
}

int	buddy_init(buddy* this, void* pool, void* bitmap_buffer){
	if (!this||!pool||!bitmap_buffer)	return -1;	/*test valid input*/
	this->pool = pool;
	bitmap_init(&(this->bitmap), 1024*((2^5)-1), bitmap_buffer);
	return 0;
}

void*	buddy_alloc(buddy* this, uint32_t size){
	size += sizeof(uint32_t);	/*get space to write block number*/
	uint8_t		level = _get_level_from_size(size);
	uint32_t	base = _get_first_of_level(level);
	int		offset = _get_bitmap_offset(&(this->bitmap),base,_get_first_of_level(level+1)-1);
	if (offset==FAILURE)
		return NULL;
	void* block = this->pool;
	block += offset*_get_level_block_size(level);
	*(uint32_t*)(block)=base+offset;
	_set_bitmap_biased(this, base+offset, 1);
	return block+sizeof(uint32_t);
}

void	buddy_free(buddy* this, void* memory);
