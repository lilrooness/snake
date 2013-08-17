snake
=====

A snake game written in C with ncurses

Lua Hooks
=========

Hooks in the C code call the lua functions eat_snack and tick
eat_snack is called when the snake eats a snack and tick is called each frame

Lua Globals
=================

You can call printhash(x, y) in lua to print a '#' to the scree
You can call growsnake() to grow the snake by one segment
WIN_HEIGHT is the screen height
WIN_WIDTH is the screen width

Compilation
===========

gcc -w -I /usr/include/lua5.2/ -llua5.2 -lncurses -o snake main.c worm.h
(or just run make)
