#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#include "buddy.h"
#include "color.h"

int main(void){
	printf("\033[2J\033[H");	/*	jedy mint trick	*/
	void* hugepage = mmap(
			NULL,
			2097152,	/*	2*1024*1024	*/
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,
			-1,
			0);
	if (hugepage==MAP_FAILED)
		return -1;
	memset(hugepage,0,2097152);
	/*	redistribution of huge page between structures	*/
	void* pool =	hugepage + 1048576;	/*	1024*1024 jump across	*/
	void* buffer =	pool - 3968;		/*	128 (1024/8) * 31 (1024|512|256|128|64 > 1+2+4+8+16=31)	*/
	if ((long)(buffer)%4)	printf("error alignment of %ld\n",(long)(buffer)%4);
	buddy* buddy =	hugepage;
	/*	|	buddy	|	..mordor..	|	bitmap_buffer	||	buddy_pool				|	*/
	#if _VERBOSE_
	printf("page:\t%s%p%s\npool:\t%s%p%s\nbuffer:\t%s%p%s\nbuddy:\t%s%p%s\n",
			GREEN,hugepage,RESET,
			ORANGE,pool,RESET,
			MAGENTA,buffer,RESET,
			GREEN,buddy,RESET);
	#endif
	buddy_init(buddy, pool, buffer);
	for(uint8_t i=0;i<16;i++){
		printf("\033[1mallocation number: \033[38;2;0;127;255m%d\033[0;1m\tsize allocated: \033[38;2;255;127;0m%4d\033[0m\n",i,1000);
		printf("\033[4;38;2;255;255;0m%p\033[0m\n",buddy_alloc(buddy,1000));
		if (i) break;
	}
	//bitmap_print(&(buddy->bitmap));
	return 0;
}
