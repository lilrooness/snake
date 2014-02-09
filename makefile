CC=gcc

CFLAGS=-w

all: snake

snake: main.c worm.h; \
        $(CC) $(CFLAGS) -I /usr/include/lua5.2 -llua5.2 -lncurses worm.h main.c -o bin/snake
       
clean: ;\
        rm -f *.o snake
