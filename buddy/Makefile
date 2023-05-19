CC=gcc
CFLAGS=--std=gnu99 -Wall -D_LIST_DEBUG_

OBJS=objs/buddy.o

BINS=test

.phony:	all clean

all:	$(BINS)

test:	buddytest.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	./$@

# who the heck knew the "-c" remove the "main" function request, lmao
%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:	
	rm -rf *.o
