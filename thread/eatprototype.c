#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#define sleeptime 800000

int cnt = 99;
typedef pthread_mutex_t mutex;
typedef pthread_t philosopher;
mutex chopstick[6] ;
void *eat_think(void *arg)
{
	char phi = *(char *)arg;
	int left,right;
	switch (phi){
		case 'A':
			left = 5;
			right = 1;
			break;
		case 'B':
			left = 1;
			right = 2;
			break;
		case 'C':
			left = 2;
			right = 3;
			break;
		case 'D':
			left = 3;
			right = 4;
			break;
		case 'E':
			left = 4;
			right = 5;
			break;
	}
 
	int i;
	while(1){
		usleep(sleeptime);
		pthread_mutex_lock(&chopstick[left]);
		if (pthread_mutex_trylock(&chopstick[right]) == EBUSY){
			pthread_mutex_unlock(&chopstick[left]);
			continue;
		}
		cnt--;
		if(cnt<0){
			break;
		}
		clear();
		mvaddstr(10,10,"      A      ");
		mvaddstr(11,10,"   /-----\\   ");
		mvaddstr(12,10,"  / /   / \\  ");
		mvaddstr(13,10,"B|         |C");
		mvaddstr(14,10,"  \\/     \\/  ");
		mvaddstr(15,10,"   \\__/__/   ");
		mvaddstr(16,10,"  D      E   ");
		char ch1 = '0'+cnt/10;
		char ch2 = '0'+cnt%10;
		mvaddch(17,19,ch1);
		mvaddch(17,20,ch2);
		if(phi=='A'){
			refresh();
			mvaddstr(10,10,"     /A/     ");
			mvaddstr(12,10,"  /       \\  ");
		}
		if(phi=='B'){
			refresh();
            mvaddstr(12,10,"--/     / \\  ");
            mvaddstr(14,10,"--\\      \\/  ");
        }
		if(phi=='D'){
            refresh();
            mvaddstr(14,10,"  \\     \\/  ");
            mvaddstr(15,10,"  |\\_____/   ");
			mvaddstr(16,10,"  D__    E   ");
        }
		if(phi=='E'){
			refresh();
            mvaddstr(14,10,"  \\/      /  ");
            mvaddstr(15,10,"   \\_____/|  ");
			mvaddstr(16,10,"  D    __E   ");
        }
		if(phi=='C'){
			refresh();
		    mvaddstr(12,10,"  / /     \\--");
        	mvaddstr(14,10,"  \\/      /--");                                       
        }
		refresh();
		usleep(sleeptime);
		pthread_mutex_unlock(&chopstick[left]);
		pthread_mutex_unlock(&chopstick[right]);
	}
}

int main(){
	philosopher A,B,C,D,E;
	int i;
	initscr();
	cbreak();
	nonl();
	echo();
	intrflush(stdscr,FALSE);
	keypad(stdscr,TRUE);
	refresh();
	box(stdscr,0,0);
	for (i = 0; i < 5; i++)
		pthread_mutex_init(&chopstick[i],NULL);
	pthread_create(&A,NULL, eat_think, "A");
	pthread_create(&B,NULL, eat_think, "B");
	pthread_create(&C,NULL, eat_think, "C");
	pthread_create(&D,NULL, eat_think, "D");
	pthread_create(&E,NULL, eat_think, "E");
	pthread_join(A,NULL);
	pthread_join(B,NULL);
	pthread_join(C,NULL);
	pthread_join(D,NULL);
	pthread_join(E,NULL);
	return 0;
}
