CC=gcc
CFLAGS=--std=gnu99 -Wall 

OBJS=objs/mhelper.o

BINS=.test

.PHONY:	clear run debug

debug:	CFLAGS:=$(CFLAGS)-D_VERBOSE_=1
debug:	$(BINS)
	./$<

.test:	mhelpertest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clear:
	rm -rf $(OBJS) $(BINS)

run:	$(BINS)
	./$<
