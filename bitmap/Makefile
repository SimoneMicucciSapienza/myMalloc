CC=gcc
CFLAGS=--std=gnu99 -Wall -D_LIST_DEBUG_

OBJS=objs/bitmap.o

BINS=test

.phony:	all clean

all:	$(BINS)

test:	bitmaptest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	./$@

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -rf objs/*.o
	rm -rf test
