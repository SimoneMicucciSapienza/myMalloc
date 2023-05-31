CC=gcc
CFLAGS=--std=gnu99 -Wall -D_LIST_DEBUG_

BINS=.test

OBJS=objs/mhelper.o

.PHONY:	all clear run

all:	$(BINS)

.test:	mhelpertest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clear:
	rm -rf $(OBJS) $(BINS)

run:	$(BINS)
	./$<
