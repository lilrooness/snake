CC=gcc

CFLAGS=-Wall 

all: snake

snake: main.c worm.h; \
        $(CC) $(CFLAGS) -lncurses worm.h main.c -o snake
       
clean: ;\
        rm -f *.o snake
