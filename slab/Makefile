CC=gcc
CFLAGS=--std=gnu99 -Wall -D_LIST_DEBUG_

OBJS=objs/slab.o\
     objs/bitmap.o

BINS=.test

.PHONY:	all clean run

run:	$(BINS)
	./$<

all:	$(BINS)

.test:	slabtest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -rf $(OBJS) $(BINS)
