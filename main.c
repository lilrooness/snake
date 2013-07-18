#include <time.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "worm.h"

#define SHROOM_CHANCE 50

void printworm(struct seg *back);
struct seg* moveworm(struct seg *back, char key);
struct seg* getfront(struct seg *n);
struct snack* gensnack(int x, int y, int points, bool shroom);
void managesnacks(bool *avail, struct snack **snk);
void eatsnacks(struct snack *snk, struct seg *front, bool *avail);
void printsnacks(struct snack *snk, bool *avail);
void growsnake(struct seg *front);
void addfrontsegment(struct seg* back, struct seg *newseg, char dir);
int points;
int main(int argc, char *argv[]) {
	points = 0;
	bool snacksavailable = false;
	struct snack *snk;
	srand(time(NULL));
	bool alive = true;
	struct seg *back = (struct seg*) initworm(20, 5, 0);
	printf("initialised worm\n");
	initscr();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	noecho();
	nodelay(stdscr, true);
	int i;
	char key;
	while(alive) {
	mvprintw(0,0,"%d", getfront(back)->x);
		managesnacks(&snacksavailable, &snk);
		eatsnacks(snk, getfront(back), &snacksavailable);
		erase();
		printsnacks(snk, &snacksavailable);
		printworm(back);
		usleep(100000);
		key = getch();
		back = moveworm(back, key);
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
	//REPLACE WITH ADD TO FRONT METHOD
	addfrontsegment(newback, back, dir);
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

struct snack* gensnack(int x, int y, int points, bool shroom) {
	struct snack *snack = (struct snack*) malloc(sizeof(struct snack));
	snack->shroom = shroom;
	snack->points = points;
	snack->x = x;
	snack->y = y;
	return snack;
}

void managesnacks(bool *avail, struct snack **snk) {
	if(!*avail) {
		*snk = gensnack(rand()%50 + 1, rand()%50 + 1, rand()%4+1, ((rand()%100) < SHROOM_CHANCE)?true:false);
		*avail = true;
	}
}

void addfrontsegment(struct seg* back, struct seg *newseg, char dir) {
	struct seg *front = getfront(back);
	front->dir = dir;
	front->front = 0;
	front->succ = newseg;
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
}

void eatsnacks(struct snack *snk, struct seg *front, bool *avail) {
	if(*avail) {
		if(front->x == snk->x && front->y == snk->y) {
			points+=snk->points;
			*avail = false;
			if(snk->shroom) {
				//do something awesome!
			}
			free(snk);
			growsnake(front);	
		}
	}
}

void printsnacks(struct snack *snk, bool *avail) {
	attron(1);
	if(*avail) {
		mvprintw(snk->y, snk->x, "#");
		mvprintw(0,0,"x:%d, y:%d", snk->x, snk->y);
	}
	attroff(1);
}

void growsnake(struct seg *back) {
	struct seg *newseg = (struct seg *) malloc(sizeof(struct seg));
	addfrontsegment(back, newseg, getfront(back)->dir);
}
