CC=gcc
CFLAGS=--std=gnu99 -Wall 

OBJS=objs/buddy.o\
     objs/slab.o\
     objs/bitmap.o\
     objs/mhelper.o\
     objs/pseudomalloc.o\

BINS=.test

.PHONY:	test run clean

.test:	pseudomalloctest.c $(OBJS)
	$(CC) $(CFLAGS) -g -rdynamic -o $@ $^

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -rf $(OBJS) $(BINS)

run:	$(BINS)
	./$<

test:	$(BINS)
