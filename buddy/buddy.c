#include "buddy.h"
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

static buddy buddy_handler;

int	MemoryInit() {
	buddy_handler.bitmap = mmap(
			NULL,
			((2<<20)+(2<<11),
			 PROT_READ|PROT_READ,
			 MAP_ANONYMOUS|,
			 -1,0);
	if (buddy_handler.bitmap == MAP_FAILED) {
		printf("mapping memory failed: %s\n",strerror(errno));
		return FAILURE;
	}
}
