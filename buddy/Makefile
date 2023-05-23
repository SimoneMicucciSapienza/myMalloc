CC=gcc
CFLAGS=--std=gnu99 -Wall -D_LIST_DEBUG_

OBJS=objs/buddy.o\
     objs/bitmap

BINS=.test

.PHONY:	all clean run

all:	$(BINS)

.test:	buddytest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

objs/bitmap:	
	make -C ../bitmap/ all
	ln -s ../bitmap/objs/bitmap.o

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -rf *.o

run:	$(BINS)
	./$<
