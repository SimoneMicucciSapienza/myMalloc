.PHONY:	all

all:
	make -C pseudomalloc run
	mv pseudomalloc/.test
