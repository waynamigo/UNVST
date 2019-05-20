#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <zconf.h>
#include <curses.h>

#define N 5

pthread_mutex_t mutex;

enum  {
    thinking,eating,hungury
} phi_sta[N];

pthread_cond_t phi_con[N];
pthread_t pthread_id[N];
int phi_num[N] = {0,1,2,3,4};
int cur_noodle_num = 99;

int chop_ori[N][2] = {{4,11},{5,18},{9,17},{9,7},{5,5}};
int chop_left[N][2] =  {{2,1},{2,19},{6,24},{13,13},{8,1}};
int chop_right[N][2] =  {{2,5},{2,23},{8,24},{13,9},{6,1}};
char chop_type[N] = {'|','|','-','|','-'};
char chop_ori_type[N] = {'|','/','\\','/','\\'};

char table[20][50] = {
" ",
" ",  
"   A                 B  ",
"      ____________",
"     /     |      \\",
"    /\\            /\\",
"   /                \\    ",
" E |       99        |  C",
"    \\               /    ",
"     \\ /         \\/",
"       \\        /",
"         \\     /",
"           --",
"           D  "
};

void test(int i){
    if (phi_sta[(i+N-1)%N]!=eating && phi_sta[i]==hungury && phi_sta[(i+1)%N]!=eating ){
        if (cur_noodle_num>0 ) {
            phi_sta[i] = eating;
          
            mvaddch(chop_ori[i][0],chop_ori[i][1],' ');
            mvaddch(chop_ori[(i+N-1)%N][0],chop_ori[(i+N-1)%N][1],' ');
            mvaddch(chop_left[i][0],chop_left[i][1],chop_type[i]);
            mvaddch(chop_right[i][0],chop_right[i][1],chop_type[i]);
            cur_noodle_num--;
            mvaddch(7,11,'0'+cur_noodle_num/10);
            mvaddch(7,12,'0'+cur_noodle_num%10);
            refresh();
            sleep(1);
            mvaddch(chop_ori[i][0],chop_ori[i][1],chop_ori_type[i]);
            mvaddch(chop_ori[(i+N-1)%N][0],chop_ori[(i+N-1)%N][1],chop_ori_type[(i+N-1)%N]);
            mvaddch(chop_left[i][0],chop_left[i][1],' ');
            mvaddch(chop_right[i][0],chop_right[i][1],' ');
            refresh();
          //  printf("philosopher %d is eating\n",i);
        }
        pthread_cond_signal(&phi_con[i]);
    }
}

void pick_up(int i){
    pthread_mutex_lock(&mutex);
    phi_sta[i]=hungury;
    test(i);
    if (phi_sta[i]!=eating) pthread_cond_wait(&phi_con[i],&mutex);
    pthread_mutex_unlock(&mutex);
    sleep(1);
}

void put_down(int i){
    pthread_mutex_lock(&mutex);
    phi_sta[i] = thinking;
    test((i+1)%N);
    test((i+N-1)%N);
    pthread_mutex_unlock(&mutex);
}

void philosopher(void *num){
    int i = *((int*)num);
   // printf("%d\n",i);
    while ( cur_noodle_num > 0  ) {
        sleep(1);
        pick_up(i);
        sleep(0);
        put_down(i);
    }
}

int main(){

    initscr();
	cbreak();
	clear();
	for (int i=0;i<14;i++){
        mvaddstr(i,0,table[i]);
    }

    refresh();
   // while(1);

    pthread_mutex_init(&mutex,NULL);
    for (int i=0;i<N;i++){
        phi_sta[i] = thinking;
        pthread_cond_init(&phi_con[i],NULL);
    //   mvaddch(chop_ori[i][0],chop_ori[i][1],chop_ori_type[i]);
        pthread_create(&pthread_id[i],NULL,philosopher,&phi_num[i]);
       // printf("philosopher %d is ready\n",i);
    }

    for (int i=0;i<N;i++){
        pthread_join(pthread_id[i],NULL);
        sleep(1);
    }

    endwin();

    return 0;
}
