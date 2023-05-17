CC=gcc
CFLAGS=--std=gnu99 -Wall -D_LIST_DEBUG_

OBJS=test

BINS=mmapping.o

.phony:	all clear

all:	$(OBJS)
	./$@

%.o:	%.c
	$(CC) $(CFLAGS) -o $@ $<

test:	mmappingtest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
