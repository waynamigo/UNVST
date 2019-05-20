#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <math.h>
#include <unistd.h>

#include "pthread.h"
#define BUFFER_SIZE 16
#define sleeptime 400000
/* Circular buffer of integers. */
typedef pthread_mutex_t mutex;
typedef pthread_cond_t signal;
typedef pthread_cond_t signal;

struct prodcons {
  mutex lock;         /* mutex ensuring exclusive access to buffer */
  int policepos, thiefpos;        /* positions for reading and writing */
  signal notempty;      /* signaled when buffer is not empty */
  signal notfull;       /* signaled when buffer is not full */
};
/*--------------------------------------------------------*/
/* Initialize a buffer */
void init(struct prodcons * b)
{
  pthread_mutex_init(&b->lock, NULL);//先lock信号量
  pthread_cond_init(&b->notempty, NULL);
  pthread_cond_init(&b->notfull, NULL);
  b->policepos =5;
  b->thiefpos = 5;
}
/*--------------------------------------------------------*/
/* Store an integer in the buffer */
int put(struct prodcons * b, int x)
{
//long sleeptime =100000;
 	pthread_mutex_lock(&b->lock);
   /* Wait until buffer is not full */
  	while (b->policepos - b->thiefpos>3) {
     		pthread_cond_wait(&b->notfull, &b->lock);
   	}
	b->policepos++;
	usleep(sleeptime);
	x=b->policepos;
  /* Write the data and advance write pointer */
  /* Signal that the buffer is now not empty */
   	pthread_cond_signal(&b->notempty);
 	pthread_mutex_unlock(&b->lock);
	return x;
}
/*--------------------------------------------------------*/
/* Read and remove an integer from the buffer */
int  get(struct prodcons * b ,int x)
{
 //long sleeptime =100000;
 	pthread_mutex_lock(&b->lock);
  	/* Wait until buffer is not empty */
  	while (b->thiefpos-b->policepos>=3) {
  		pthread_cond_wait(&b->notempty, &b->lock);
   	}
	b->thiefpos++;
	usleep(sleeptime);
	x=b->thiefpos;
   /* Read the data and advance read pointer */
   /* Signal that the buffer is now not full */
   	pthread_cond_signal(&b->notfull);
   	pthread_mutex_unlock(&b->lock);
   	return x;
}
/*--------------------------------------------------------*/
//#define OVER (-1)
struct prodcons buffer;
/*--------------------------------------------------------*/
void * policefunc()
{
    int x1=5;
    int y=1;
    int n=50000;
    char pos='P';
    initscr();
    crmode();
    noecho();
    while(1)
    {
        move(y,0);
        clrtoeol();
        mvaddch(y,x1,pos);
        refresh();
        x1= put(&buffer,x1);
        if(x1==200)break;
    }
    mvaddch(y,x1,pos);
    refresh();
    usleep(sleeptime);
    endwin();
    return NULL;
}
void * thieffunc()
{
    
    int x2=5;
    int y=2;
    int n=50000;
    char pos='T';
    while(1)
    {
        move(y,0);
        clrtoeol();
        mvaddch(y,x2,pos);
        refresh();
        x2=get(&buffer,x2);
        if(x2==100)break;
    }
    clear();
    move(y,0);
    clrtoeol();
    mvaddch(y,x2,pos);
    refresh();
    usleep(sleeptime);
    endwin();
    return NULL;
}
asmlinkage int sys_print_info(int row,int col) {
    int a=5,b=6;
    int sum = a*b;
    char matrix[a+1][b+1];
    int base = 65;
    int count =0;
    int layer=0;
    int i =0,j=0;
    while(sum>=0){
        for(;j<b-layer;j++){
            sum--;
            if(sum<0) break;
            matrix[i][j] = count%26+base;
            count++;
        }
        j--;i++;
        for(;i<a-layer;i++){
            sum--;
            if(sum<0) break;
            matrix[i][j] = count%26+base;
            count++;
        }
        j--;i--;
        for(;j>=layer;j--){
            sum--;
            if(sum<0) break;
            matrix[i][j] = count%26+base;
            count++;
        }
        layer++;
        j++;i--;
        for(;i>=layer;i--){
            sum--;
            if(sum<0) break;
            matrix[i][j] = count%26+base;
            count++;
        }
        i++;j++;
    }
    for(int n=0;n<a;n++){
        for(int m=0;m<b;m++){
            printk(" %c \n",matrix[n][m]);
        }
        printk('\n');
    }
    return 0;
}
int main(int argc, char *argv[])
{
    pthread_t police,thief;
    void * retval;
    init(&buffer);
    pthread_create(&police, NULL, policefunc, 0);
    pthread_create(&thief, NULL, thieffunc, 0);

    pthread_join(police, &retval);
    pthread_join(thief, &retval);
    return 0;
}

