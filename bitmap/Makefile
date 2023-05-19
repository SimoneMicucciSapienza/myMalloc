CC=gcc
CFLAGS=--std=gnu99 -Wall -D_LIST_DEBUG_

OBJS=objs/buddy.o

BINS=test

.phony:	all clean

all:	$(BINS)

test:	buddytest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	./$@

%.o:	%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:	
	rm -rf *.o
