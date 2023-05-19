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
	uint8_t	*buffer;
	int	buffer_size;
	int	num_bits;
} BitMap;

//	bitmap init
void	BitMap_init(BitMap* this, int num_bits, uint8_t* buffer);

//	bitmap set
void	BitMap_set(BitMap* this, int pos_bit, uint8_t status);

//	bitmap get
uint8_t	BitMap_get(BitMap* this, int pos_bit);

//	bitmap destroy
void	BitMap_destroy(BitMap* this);

//	bitmap print
void	BitMap_print(BitMap* this);

//	bitmap 64bit get
int	BitMap_getBytes(BitMap* this, int pos_bit);

/*
 * 	0001	0010	0100	1000
 * 	1	2	4	8
 * 	1110	1101	1011	0111
 * 	e	d	b	7
