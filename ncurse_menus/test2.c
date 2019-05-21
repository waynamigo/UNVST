#include <curses.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#define MAX(a,b) a>b?a:b;
#define MIN(a,b) a<b?a:b;

int ballx=10;
int bally=10;
int direction=1;
char ball='O';
int barx;
int bary;
char *bar="__________";
int barlength=10;

int set_ticker(long n_msecs)
{
struct itimerval new_timeset;
long n_sec,n_usecs;
n_sec=n_msecs/1000;
n_usecs=(n_msecs%1000)*1000L;
new_timeset.it_interval.tv_sec=n_sec;
new_timeset.it_interval.tv_usec=n_usecs;
new_timeset.it_value.tv_sec=n_sec;
new_timeset.it_value.tv_usec=n_usecs;
return setitimer(ITIMER_REAL,&new_timeset,NULL);
}

void paint()
{
chtype a;
clear();
mvaddch(bally,ballx,ball);
mvaddstr(bary,barx,bar);

refresh();
ballx+=direction;
if(ballx==COLS)
{direction=-1;ballx=COLS-1;beep();}
if(ballx<0)
{direction=1;ballx=0;beep();}

a=getch();
switch(a)
{
case KEY_UP :
{
barx=barx+1;
//barx=MIN(barx+1,COLS-1-barlength);
break;}
case KEY_DOWN :
{
barx=barx-1;
//barx=MAX(barx-1,0);
break;}
}


//barx=barx+1;
}


int main(int argc, char *argv[])
{
chtype input;
long delay=100;
initscr();
bary=LINES-1;
barx=10;
crmode();
noecho();

signal(SIGALRM,paint);
set_ticker(delay);

while((input=getch())&&input!=ERR&&input!='q')
{
//signal(SIGALRM,paint);
//set_ticker(delay);

switch(input)
{
case 'f':
{delay /=2; set_ticker(delay);break;}
case 's':
{delay *=2; set_ticker(delay);break;}
case KEY_UP :
{
barx=barx+1;
//barx=MIN(barx+1,COLS-1-barlength);
break;}
case KEY_DOWN :
{
barx=barx-1;
//barx=MAX(barx-1,0);
break;}
}}
endwin();
printf("%d",barx);
return 0;
}
