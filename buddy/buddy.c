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
/*	from the block number return the current level of buddy request*/
static inline uint8_t _get_level_from_block(uint32_t block_number){
	uint8_t res=0;
	while (block_number){
		block_number = block_number>>1;
		res++;
	}
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
		printf("%d",(prt[3-(i/8)] & print_mask[i%8])?1:0);
		if (i%4==3)	printf(" ");
	}
	printf("\n");
}
#endif
/*	bitmap parser to reach offset*/
static inline int _get_bitmap_offset(bitmap *bitmap, uint32_t start, uint32_t end){	/*here we use bitmap bias*/
	uint32_t bitmap_chunk = _reverse_endianness(bitmap_getBytes(bitmap, start - BITMAP_BIAS));
	uint32_t offset = 0, max_offset = end - start;
	while (bitmap_chunk == 0xffffffff){
		#if _VERBOSE_
		printf("\t%s%04d%s: ",AZURE,offset,RESET);
		_print_bytes(bitmap_chunk);
		#endif
		offset +=32;
		bitmap_chunk = _reverse_endianness(bitmap_getBytes(bitmap, start - BITMAP_BIAS + offset));
	}
	while (bitmap_chunk>0x7fffffff && bitmap_chunk){
		#if _VERBOSE_
		printf("\t%s%04d%s: ",AZURE,offset,RESET);
		_print_bytes(bitmap_chunk);
		#endif
		bitmap_chunk = bitmap_chunk << 1;
		offset++;
	}
	if (offset > max_offset)
		return FAILURE;
	return offset;
}
/*	set bitmap helper wt parent and child setting*/
static inline void _set_bitmap(buddy* this, int buddy_num, uint8_t status){
	/*	set requirment to iterate	*/
	uint32_t queue[32];	/*	extra to max number of child for this specific buddy	*/
	uint8_t queue_head = 0,
		queue_tail = 0,
		queue_size = 0;	/*	queue element required to manage it	*/
	/*	set to queue current buddy and manage setting	*/
	queue[queue_tail] = buddy_num;
	queue_size++;	queue_tail = (queue_tail+1)%32;		/*	push elem to queue	*/
	/*	allocation parent circuit	*/
	if (status){
		uint8_t parent_level = _get_level_from_block(buddy_num);	/*get level of current block*/
		while (parent_level>10){	/*level 10 buddy is not avaible by setting*/
			bitmap_set(&(this->bitmap),queue[queue_head] - BITMAP_BIAS, status);	/*set current queue head*/
			#if _VERBOSE_
				printf("%s%04d(%1d)%s ",MAGENTA,queue[queue_head]-BITMAP_BIAS,(status),RESET);
			#endif
			if (--parent_level>10){	/* preincrement to reduce lines	*/
				queue[queue_tail] = queue[queue_head] >> 1;		/*	set parent to queue	*/
				queue_size++;	queue_tail = (queue_tail+1)%32;		/*	push elem to queue	*/
			}
			queue_size--;	queue_head = (queue_head+1)%32;		/*	draw elem to queue	*/
		}
	}
	/*	deallocation parent circuit	*/
	else {
		uint8_t parent_level = _get_level_from_block(buddy_num);	/*get level of current block*/
		while (parent_level>11){	/*level 10 buddy is not avaible by setting*/
			uint32_t buddy = queue[queue_head] + ((queue[queue_head]%2)? 1 : -1);
			bitmap_set(&(this->bitmap),queue[queue_head] - BITMAP_BIAS, status);	/*set current queue head*/
			#if _VERBOSE_
				printf("%s%04d(%1d)%s ",MAGENTA,queue[queue_head]-BITMAP_BIAS,(status),RESET);
			#endif
			if (++parent_level>11){	/* preincrement to reduce lines	*/
				if (bitmap_get(&(this->bitmap),buddy-BITMAP_BIAS)){	/*	if buddy to 1	*/
					queue_size--;	queue_head = (queue_head+1)%32;		/*	draw elem to queue	*/
					break;		/*	bookkeeping of structure after break	*/
				}
				queue[queue_tail] = queue[queue_head] >> 1;		/*	set parent to queue	*/
				queue_size++;	queue_tail = (queue_tail+1)%32;		/*	push elem to queue	*/
			}
			queue_size--;	queue_head = (queue_head+1)%32;		/*	draw elem to queue	*/
		}
	}
	#if _VERBOSE_
		printf("%s(parents ended)%s\n",PURPLE,RESET);
	#endif
	/*	child set circuit	*/
	queue[queue_tail] = buddy_num;
	queue_size++;	queue_tail = (queue_tail+1)%32;		/*	push elem to queue	*/
	while (queue_size){	/*	child setting	*/
		bitmap_set(&(this->bitmap),queue[queue_head] - BITMAP_BIAS, status);	/*set current queue head*/
		#if _VERBOSE_
			printf("%s%04d(%1d)%s ",MAGENTA,queue[queue_head]-BITMAP_BIAS,(status),RESET);
		#endif
		if (_get_level_from_block(queue[queue_head])<=MAX_LEVEL){
			queue[queue_tail] = queue[queue_head]<<1;
			queue_size++;	queue_tail = (queue_tail+1)%32;		/*	push elem to queue	*/
			queue[queue_tail] = (queue[queue_head]<<1) + 1;
			queue_size++;	queue_tail = (queue_tail+1)%32;		/*	push elem to queue	*/
		}
		queue_size--;	queue_head = (queue_head+1)%32;		/*	push elem to queue	*/
	}
	#if _VERBOSE_
		printf("%s(children ended)%s\n",PURPLE,RESET);
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
	bitmap_init(&(this->bitmap), 1024*(31), bitmap_buffer);
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
	_set_bitmap(this, base+offset, 1);
	return block+sizeof(uint32_t);
}

void	buddy_free(buddy* this, void* memory);
