#include <stdio.h>
#include <sys/mman.h>

#include "mhelper.h"

int main (int argc, char* argv[]) {
	(void)printf("mem:%ld\npage:%ld\n",sizeof(mem),sizeof(page));
	return 0;
}
