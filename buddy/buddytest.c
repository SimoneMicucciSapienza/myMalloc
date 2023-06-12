#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

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
	sleep(1);
	uint32_t ssizee = 200;
	printf("\033[4;38;2;255;255;0m%p\033[0m\n",buddy_alloc(buddy,50));
	uint8_t round = 32;
	void* allocation[round];
	for(uint8_t i=0;i<round;i++){
		printf("\033[1mallocation number: \033[38;2;0;127;255m%d\033[0;1m\tsize allocated: \033[38;2;255;127;0m%4d\033[0m\n",i,ssizee);
		allocation[i] = buddy_alloc(buddy,ssizee);
		printf("\033[4;38;2;255;255;0m%p\033[0m\n",allocation[i]);
		sleep(1);
	}
	sleep(10);
	bitmap_print(&(buddy->bitmap));
	sleep(1);
	for(uint8_t i=0;i<round;i++){
		if (i%2)	continue;
		printf("\033[1mdeallocation number: \033[38;2;0;127;255m%d\033[0m\n",i);
		buddy_free(buddy,allocation[i]);
		sleep(1);
	}
	sleep(10);
	bitmap_print(&(buddy->bitmap));
	sleep(1);
	for(uint8_t i=0;i<round;i++){
		if (!(i%2))	continue;
		printf("\033[1mdeallocation number: \033[38;2;0;127;255m%d\033[0m\n",i);
		buddy_free(buddy,allocation[i]);
		sleep(1);
	}
	sleep(10);
	bitmap_print(&(buddy->bitmap));
	sleep(1);
	return 0;
}
