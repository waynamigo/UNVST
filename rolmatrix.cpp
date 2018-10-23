#include<iostream>
#include<ncurses.h>
using namespace std;
const int x=20,y=5;
void rolmatrix(){
	int a,b;
	cout<<"input row:";
	scanf("%d%d",&a,&b);
	cout<<a<<b<<endl;
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
			mvprintw(y+n,x+m,"%c",matrix[n][m]);
		}
		cout<<endl;
	}
	getchar();
	refresh(); 
}
inline void init_curses()
{
    initscr();
	cbreak();
	nonl();
	echo();
	intrflush(stdscr,FALSE);
	keypad(stdscr,TRUE);
	refresh();
}
int main()
{
	init_curses();
	box(stdscr,0,0);
	int count =1;
	while(true){
		if(count==1){

			//attron(A_REVERSE);
			mvaddstr(0,25,"input number to check homework");
			//attroff(A_REVERSE);
			mvaddstr(5,30,"[1]roltedMatrix");
		}
		count++;
		char ch = getch();
		switch(ch){
			case '1':
				rolmatrix();
				break;
			case '2':
				mvaddstr(6,30,"test");
				break;
			default:
				break;
		}
		move(7,30);
	}
	return 0;
}
