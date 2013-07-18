#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "worm.h"

void printworm(struct seg *back);
struct seg* moveworm(struct seg *back, char key);
struct seg* getfront(struct seg *n);


int main(int argc, char *argv[]) {
	bool alive = true;
	struct seg *back = (struct seg*) initworm(20, 5, 0);
	printf("initialised worm\n");
	printf("%c", 'u');
	initscr();
	noecho();
	nodelay(stdscr, true);
	int i;
	char key;
	while(alive) {
	mvprintw(0,0,"%d", getfront(back)->x);
		erase();
		printworm(back);
		key = getch();
		back = moveworm(back, key);
		usleep(100000);
		refresh();
	}
	endwin();

	delsegs(back);
	return 0;
}

void printworm(struct seg *back) {
	int front = 0;
	struct seg *cur = back;
	while(front != 1) {
		front = cur->front;
		mvprintw(cur->y, cur->x, "#");
		cur = cur->succ;
	}
}

struct seg* moveworm(struct seg *back, char key) {
	struct seg* newback = back->succ;
	struct seg *front = getfront(newback);
	char dir = front->dir;
	if(key == RIGHT) {
		dir = RIGHT;
	}else if(key == LEFT) {
		dir = LEFT;
	}else if(key == UP) {
		dir = UP;
	}else if(key == DOWN) {
		dir = DOWN;
	}
	front->dir = dir;
	front->front = 0;
	front->succ = back;
	switch(front->dir) {
		case RIGHT:{
			front->succ->x = front->x + 1;
			front->succ->y = front->y;
		}break;
		case LEFT:{
			front->succ->x = front->x - 1;
			front->succ->y = front->y;
		}break;
		case UP:{
			front->succ->x = front->x;
			front->succ->y = front->y - 1;
		}break;
		case DOWN:{
			front->succ->x = front->x;
			front->succ->y = front->y + 1;
		}
	}
	front->succ->dir = front->dir;
	front->succ->front = 1;
	return newback;
}

struct seg* getfront(struct seg *n) {
	int front =0;
	struct seg *cur;
	cur = n;
	while(cur->front != 1){
		cur = cur->succ;
	}
	return cur;
}
