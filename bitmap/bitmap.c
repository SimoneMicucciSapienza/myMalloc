#include "bitmap.h"
#include "color.h"

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

// like avr table
static const uint8_t _set_up_table [] =
{
	0xfe,
	0x7f,
	0xbf,
	0xdf,
	0xef,
	0xf7,
	0xfb,
	0xfd
};

static const uint8_t _set_dw_table [] =
{
	0x01,
	0x80,
	0x40,
	0x20,
	0x10,
	0x08,
	0x04,
	0x02
};

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
	//smarter way to set bit and more readable (if you ignore the tables)
	if (status)
		this->buffer[(pos_bit/8)-(!(pos_bit%8))] |= _set_dw_table [pos_bit%8];
	else
		this->buffer[(pos_bit/8)-(!(pos_bit%8))] &= _set_up_table [pos_bit%8];
	return;
}

//	bitmap get
uint8_t	BitMap_get(BitMap* this, int pos_bit){
	if (this->num_bits < pos_bit)
		return 2;
	uint8_t target = this->buffer[(pos_bit/8) - (!pos_bit%8)];
	//smart way to get bit and more readable
	return (uint8_t)(target & _set_dw_table[pos_bit%8]);
}

//	bitmap print
void	BitMap_print(BitMap* this){
	//shit test wt a printf, M A J E S T I C
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
		dprintf(STDERR_FILENO,"%sERROR%s null pointer exception\n",RED,RESET);
		raise(SIGKILL);
		pause();
	}
	if (this->num_bits<pos_bit){
		//sadly I din't find any other way to comunicate a significative message over kill program
		dprintf(STDERR_FILENO,"%sERROR%s request out of boundaries\n",ORANGE,RESET);
		raise(SIGKILL);
		pause();
	}
	//not this big trouble, require testing in the user to check fast scanning
	int* tmp = (int*)(this->buffer);
	return tmp[(pos_bit/32)-(!(pos_bit%32))];
}
