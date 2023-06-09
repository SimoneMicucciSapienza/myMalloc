#include <stdio.h>
#include "color.h"
#include "bitmap.h"

uint8_t buffer[8];

int main(int argv, char*argc[]) {
	printf("initialize bit map:\n");
	bitmap bm0;
	for (int i=0;i<8;i++){
		buffer[i]=0x00;
	}
	bitmap_init(&bm0, 64, (uint8_t*)(&buffer));
	bitmap_print(&bm0);
	printf("change first Byte to 1 all bits\n");
	for (int i=1;i<9;i++){
		printf("\nset bit %s%d%s\n",RED,i,RESET);
		bitmap_set(&bm0,i,1);
		bitmap_print(&bm0);
	}
	printf("\nreset only even bits\n");
	for (int i=2;i<9;i+=2){
		printf("\nset bit %s%d%s to 0\n",ORANGE,i,RESET);
		bitmap_set(&bm0,i,0);
		bitmap_print(&bm0);
	}
	printf("\nreset last 4 bits\n");
	for (int i=5;i<9;i++){
		printf("\nset bit %s%d%s to 0\n",ORANGE,i,RESET);
		bitmap_set(&bm0,i,0);
		bitmap_print(&bm0);
	}
	printf("check if bytes are the same:\n");
	printf("bytes 0: %s%08x%s\n",YELLOW,bitmap_getBytes(&bm0,8),RESET);
	printf("bytes 1: %s%08x%s\n",YELLOW,bitmap_getBytes(&bm0,16),RESET);
	printf("bytes 2: %s%08x%s\n",YELLOW,bitmap_getBytes(&bm0,24),RESET);
	printf("bytes 3: %s%08x%s\n",YELLOW,bitmap_getBytes(&bm0,32),RESET);
	return 0;
}
