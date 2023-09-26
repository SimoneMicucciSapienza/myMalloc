CC=gcc
CFLAGS=--std=gnu99 -Wall 

OBJS=objs/buddy.o\
     objs/slab.o\
     objs/mhelper.o\
     objs/pseudomalloc.o\

BINS=.test

.PHONY:	clean test run

.test:	pseudotester.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

objs/%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -rf $(OBJS) $(BINS)

run:	$(BINS)
	./$<

test:	$(BINS)
