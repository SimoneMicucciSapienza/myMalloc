/*
 * Reference:
 * https://gitlab.com/grisetti/sistemi_operativi_2022_23/
 * 	source/06_memory_allocation/buddy_allocator/
 * by Giorgio Grisetti
 **/

#pragma once
#include <stdint.h>

//	bitmap struct definition
//	we start counting bits by 1, if you want 8b (1 to 8) you need 1B
//	Byte:	1 0 1 0	1 0 1 1		1 0 1 0	1 1 0 1		1 1 0 0	1 0 1 0		1 1 1 1	1 1 1 0
//	Hex:	A	B		A	D		C	A		F	E
//	Pos:	1 2 3 4	5 6 7 8		1 2 3 4	5 6 7 8		1 2 3 4	5 6 7 8		1 2 3 4	5 6 7 8
//	Map:	1 2 3 4	5 6 7 8		9 10111213141516	1718192021222324	2526272829303132
typedef struct {
	uint8_t		*buffer;
	uint32_t	buffer_size;
	uint32_t	num_bits;
} bitmap;

//	bitmap init
void	bitmap_init(bitmap* this, int num_bits, uint8_t* buffer);

//	bitmap set
void	bitmap_set(bitmap* this, int pos_bit, uint8_t status);

//	bitmap get
uint8_t	bitmap_get(bitmap* this, int pos_bit);

//	bitmap print
void	bitmap_print(bitmap* this);

//	bitmap 64bit get
uint32_t	bitmap_getBytes(bitmap* this, int pos_bit);

/*
 * 	0001	0010	0100	1000
 * 	1	2	4	8
 * 	1110	1101	1011	0111
 * 	e	d	b	7
 */
