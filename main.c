#include <time.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "worm.h"
#include <sys/ioctl.h>

#define SHROOM_CHANCE 50

void printworm(seg *back);
seg* moveworm(seg *back, char key);
seg* getfront(seg *n);
snack* gensnack(int x, int y, int points, bool shroom);
void managesnacks(bool *avail, snack **snk);
void eatsnacks(snack *snk, seg *front, bool *avail);
void printsnacks(snack *snk, bool *avail);
void growsnake(seg *front);
void addfrontsegment(seg* back, seg *newseg, char dir);
bool checkcollisions(seg* back);
void endgame(seg *back);
bool selfintersection(seg *back);

#define NORMAL 0
#define CRAZY_COLOR 1
#define NUTMEG 2
#define WHATEVEN 3

int effecttimecounter = 10; //the amount of ticsk shrooms have an effect for
int points;
struct winsize w;
int mode;

int main(int argc, char *argv[]) {
	mode = NORMAL;
	ioctl(0, TIOCGWINSZ, &w);
	printf("width:%d, height:%d", w.ws_row, w.ws_col);
	points = 0;
	bool snacksavailable = false;
	snack *snk;
	srand(time(NULL));
	bool alive = true;
	seg *back = (seg*) initworm(20, 5, 0);

	printf("initialised worm\n");

	initscr();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	noecho();
	nodelay(stdscr, true);
	char key;

	while(alive) {
		mvprintw(0,0,"%d", getfront(back)->x);

		managesnacks(&snacksavailable, &snk);
		eatsnacks(snk, getfront(back), &snacksavailable);

		erase();

		printsnacks(snk, &snacksavailable);
		printworm(back);
		
		if(checkcollisions(back) == true) {
			alive = false;
		}
		if(mode == NORMAL) {
			usleep(100000);
		}
		key = getch();
		back = moveworm(back, key);
		
		refresh();
	}
	endgame(back);
	return 0;
}

void endgame(seg *back) {
	endwin();
	delsegs(back);
}

void printworm(seg *back) {
	int front = 0;
	seg *cur = back;
	if(mode == NORMAL){
		while(front != 1) {
			front = cur->front;
			mvprintw(cur->y, cur->x, "#");
			cur = cur->succ;
		}
	}
}

seg* moveworm(seg *back, char key) {
	seg* newback = back->succ;
	seg *front = getfront(newback);
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

seg* getfront(seg *n) {
	seg *cur;
	cur = n;
	while(cur->front != 1){
		cur = cur->succ;
	}
	return cur;
}

snack* gensnack(int x, int y, int points, bool shroom) {
	snack *a_snack = (snack*) malloc(sizeof(snack));
	a_snack->shroom = shroom;
	a_snack->points = points;
	a_snack->x = x;
	a_snack->y = y;
	return a_snack;
}

void managesnacks(bool *avail, snack **snk) {
	if(!*avail) {
		*snk = gensnack(rand()%w.ws_col, rand()%w.ws_row, rand()%4+1, ((rand()%100) < SHROOM_CHANCE)?true:false);
		*avail = true;
	}
}

void addfrontsegment(seg* back, seg *newseg, char dir) {
	seg *front = getfront(back);
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

void eatsnacks(snack *snk, seg *front, bool *avail) {
	if(*avail) {
		if(front->x == snk->x && front->y == snk->y) {
			points+=snk->points;
			*avail = false;
			if(snk->shroom) {
			//	mode = NUTMEG;
			}
			free(snk);
			growsnake(front);	
		}
	}
}

void printsnacks(snack *snk, bool *avail) {
	attron(COLOR_PAIR(1));
	if(*avail) {
		mvprintw(snk->y, snk->x, "#");
		mvprintw(0,0,"x:%d, y:%d", snk->x, snk->y);
	}
	attroff(COLOR_PAIR(1));
}

void growsnake(seg *back) {
	seg *newseg = (seg *) malloc(sizeof(seg));
	addfrontsegment(back, newseg, getfront(back)->dir);
}

//returns true if self intersection is true or if bounderies are reached
bool checkcollisions(seg *back) {
	//first check wall collisions
	seg *front = getfront(back);
	if(front->x > w.ws_col || front->x < 0 || front->y > w.ws_row || front->y < 0) {
		return true;
	}else if(selfintersection(back)){
		return true;
	}else {
		return false;
	}
}

//returns true if the head is touching any part of the body
bool selfintersection(seg *back) {
	seg *front = getfront(back);
	seg *current = back;
	while(current->front == 0) {
		if(current->x == front->x && current->y == front->y) {
			return true;
		}else {
			current = current->succ;
		}
	}
	return false;
}
