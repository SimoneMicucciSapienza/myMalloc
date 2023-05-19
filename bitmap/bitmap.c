#include "bitmap.h"

#include <stdio.h>

//	bitmap init
void	BitMap_init(BitMap* this, int num_bits, uint8_t* buffer){
	this->buffer = buffer;
	this->num_bits = num_bits;
	this->buffer_size = (num_bits/8) + ((num_bits%8)!=0);
}

//	bitmap set
void	BitMap_set(BitMap* this, int pos_bit, uint8_t status){
	//shit test success
	if (!this)
		return;
	if (this->num_bits < pos_bit)
		return;
	//setting bit with switch map
	switch (pos_bit%8){
		case 0:			//8^ bit, slide back: 0000-0001 & 1111-1110 (01 & fe)
			if (status)
				this->buffer[pos_bit/8 -1] |= 0x01;
			else
				this->buffer[pos_bit/8 -1] &= 0xfe;
			return;
		case 1:			//1^ bit: 1000-0000 & 0111-1111 (80 & 7f)
			if (status)
				this->buffer[pos_bit/8 ] |= 0x80;
			else
				this->buffer[pos_bit/8 ] &= 0x7f;
			return;
		case 2:			//2^ bit: 0100-0000 & 1011-1111 (40 & bf)
			if (status)
				this->buffer[pos_bit/8 ] |= 0x40;
			else
				this->buffer[pos_bit/8 ] &= 0xbf;
			return;
		case 3:			//3^ bit: 0010-0000 & 1101-1111 (20 & df)
			if (status)
				this->buffer[pos_bit/8 ] |= 0x20;
			else
				this->buffer[pos_bit/8 ] &= 0xdf;
			return;
		case 4:			//4^ bit: 0001-0000 & 1110-1111 (10 & ef)
			if (status)
				this->buffer[pos_bit/8 ] |= 0x10;
			else
				this->buffer[pos_bit/8 ] &= 0xef;
			return;
		case 5:			//5^ bit: 0000-1000 & 1111-0111 (08 & f7)
			if (status)
				this->buffer[pos_bit/8 ] |= 0x08;
			else
				this->buffer[pos_bit/8 ] &= 0xf7;
			return;
		case 6:			//6^ bit: 0000-0100 & 1111-1011 (04 & fb)
			if (status)
				this->buffer[pos_bit/8 ] |= 0x04;
			else
				this->buffer[pos_bit/8 ] &= 0xfb;
			return;
		case 7:			//7^ bit: 0000-0010 & 1111-1101 (02 & fd)
			if (status)
				this->buffer[pos_bit/8 ] |= 0x02;
			else
				this->buffer[pos_bit/8 ] &= 0xfd;
			return;
	}
}

//	bitmap get
uint8_t	BitMap_get(BitMap* this, int pos_bit){
	if (this->num_bits < pos_bit)
		return 2;
	uint8_t ret = this->buffer[(pos_bit/8)+((pos_bit%8)!=0)]
}

//	bitmap destroy
void	BitMap_destroy(BitMap* this){
}

//	bitmap print
void	BitMap_print(BitMap* this){
}

//	bitmap 64bit get
int	BitMap_getBytes(BitMap* this, int pos_bit){
}
