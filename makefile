CC=gcc

CFLAGS=-w

all: snake

snake: main.c worm.h; \
        $(CC) $(CFLAGS) -I /usr/include/lua5.2 -llua5.2 -lncurses worm.h main.c -o snake
       
clean: ;\
        rm -f *.o snake

osx:
	$(CC) $(CFLAGS) -I /usr/include/lua52 -llua -lncurses worm.h main.c -o snake
