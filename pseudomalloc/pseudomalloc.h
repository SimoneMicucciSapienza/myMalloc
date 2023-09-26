#pragma once

#include <stdio.h>

#include "buddy.h"
#include "slab.h"
#include "mhelper.h"

/*	declaration of structural helper	*/
typedef struct {
	mhelper		mhelper;
	buddy		buddy;
	slab		big_slab;
	slab		small_slab;
} allocator_helper;

void	pseudoCreate();
void	pseudoDestroy();
void*	pseudoAlloc(size_t size);
char	pseudoFree(void* mem);
