#include "bitmap.h"
#include "color.h"

#include <unistd.h>
#include <signal.h>
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
	if (!this || !pos_bit)
		return;
	if (this->num_bits < pos_bit)
		return;
	//setting bit with switch map
	switch (pos_bit%8){
		case 0:			//8^ bit, slide back: 0000-0001 & 1111-1110 (01 & fe)
			if (status)
				this->buffer[(pos_bit/8) -1] |= 0x01;
			else
				this->buffer[(pos_bit/8) -1] &= 0xfe;
			return;
		case 1:			//1^ bit: 1000-0000 & 0111-1111 (80 & 7f)
			if (status)
				this->buffer[pos_bit/8] |= 0x80;
			else
				this->buffer[pos_bit/8] &= 0x7f;
			return;
		case 2:			//2^ bit: 0100-0000 & 1011-1111 (40 & bf)
			if (status)
				this->buffer[pos_bit/8] |= 0x40;
			else
				this->buffer[pos_bit/8] &= 0xbf;
			return;
		case 3:			//3^ bit: 0010-0000 & 1101-1111 (20 & df)
			if (status)
				this->buffer[pos_bit/8] |= 0x20;
			else
				this->buffer[pos_bit/8] &= 0xdf;
			return;
		case 4:			//4^ bit: 0001-0000 & 1110-1111 (10 & ef)
			if (status)
				this->buffer[pos_bit/8] |= 0x10;
			else
				this->buffer[pos_bit/8] &= 0xef;
			return;
		case 5:			//5^ bit: 0000-1000 & 1111-0111 (08 & f7)
			if (status)
				this->buffer[pos_bit/8] |= 0x08;
			else
				this->buffer[pos_bit/8] &= 0xf7;
			return;
		case 6:			//6^ bit: 0000-0100 & 1111-1011 (04 & fb)
			if (status)
				this->buffer[pos_bit/8] |= 0x04;
			else
				this->buffer[pos_bit/8] &= 0xfb;
			return;
		case 7:			//7^ bit: 0000-0010 & 1111-1101 (02 & fd)
			if (status)
				this->buffer[pos_bit/8] |= 0x02;
			else
				this->buffer[pos_bit/8] &= 0xfd;
			return;
	}
}

//	bitmap get
uint8_t	BitMap_get(BitMap* this, int pos_bit){
	if (this->num_bits < pos_bit)
		return 2;
	uint8_t target = this->buffer[(pos_bit/8) - (!pos_bit%8)];
	switch (pos_bit%8) {		//not best implementation but good enouth for now
		case 0 :		//8^ bit:
			return (uint8_t)(target & 0x01);
		case 1 :		//1^ bit:
			return (uint8_t)(target & 0x80);
		case 2 :		//2^ bit:
			return (uint8_t)(target & 0x40);
		case 3 :		//3^ bit:
			return (uint8_t)(target & 0x20);
		case 4 :		//4^ bit:
			return (uint8_t)(target & 0x10);
		case 5 :		//5^ bit:
			return (uint8_t)(target & 0x08);
		case 6 :		//6^ bit:
			return (uint8_t)(target & 0x04);
		case 7 :		//7^ bit:
			return (uint8_t)(target & 0x02);
	}
	return 2;
}

//	bitmap print
void	BitMap_print(BitMap* this){
	printf("bitmap:\t%s%p%s",(!this)?ORANGE:YELLOW,this,RESET);
	if (!this){
		printf("\n");
		return;
	}
	printf("\tbits:\t%s%d%s\tbytes:\t%s%d%s\n",GREEN,this->num_bits,RESET,GREEN,this->buffer_size,RESET);
	char* ptr = (char*)this->buffer;
	printf("buffer content:");
	//a line contain 4 byte, aka 8 hex
	int cnt = 0;
	while (1) {
		if (cnt==this->buffer_size)
			break;
		if (!(cnt%4))
			printf("\n\t");
		char hex[9]="0000 0000";
		if ((*ptr & 0x80))	hex[0]='1';	//set bit 1
		if ((*ptr & 0x40))	hex[1]='1';	//set bit 1
		if ((*ptr & 0x20))	hex[2]='1';	//set bit 1
		if ((*ptr & 0x10))	hex[3]='1';	//set bit 1
		if ((*ptr & 0x08))	hex[5]='1';	//set bit 1
		if ((*ptr & 0x04))	hex[6]='1';	//set bit 1
		if ((*ptr & 0x02))	hex[7]='1';	//set bit 1
		if ((*ptr & 0x01))	hex[8]='1';	//set bit 1
		printf("%s\t",hex);
		ptr++;
		cnt++;
	}
	printf("\n");
}

//	bitmap 64bit get
uint32_t	BitMap_getBytes(BitMap* this, int pos_bit){
	//shit test (whitout mercy :-D )
	if (!this){
		//sadly I din't find any other way to comunicate a significative message over kill program
		fprintf(STDERR_FILENO,"%sERROR%s null pointer exception\n",RED,RESET);
		raise(SIGKILL);
		pause();
	}
	if (this->num_bits<pos_bit){
		//sadly I din't find any other way to comunicate a significative message over kill program
		fprintf(STDERR_FILENO,"%sERROR%s request out of boundaries\n",ORANGE,RESET);
		raise(SIGKILL);
		pause();
	}
	//not this big trouble, require testing in the user to check fast scanning
	int* tmp = (int*)(this->buffer);
	return tmp[(pos_bit/32)-(!(pos_bit%32))];
}
