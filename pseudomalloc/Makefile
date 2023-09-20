CC=gcc
CFLAGS=--std=gnu99 -Wall 

OBJS=objs/buddy.o\

BINS=.test

.PHONY:	clean run debug

debug:	CFLAGS:=$(CFLAGS)-D_VERBOSE_=1
debug:	$(BINS)
	./$<

.test:	buddytest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -rf $(OBJS) $(BINS)

run:	$(BINS)
	./$<
