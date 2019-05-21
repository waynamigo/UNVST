#include <curses.h>
#include <string.h>

#define StartX 24
#define StartY 2

void initial();

main(){
 int x=StartX,y=StartY,ch;
 initial();
 box(stdscr,0,0);
 attron(A_REVERSE);
 mvaddstr(0,25,"1607020223 wfy");
 attroff(A_REVERSE);
 mvaddstr(2,23,"[*] showSpiralMtrix");
 mvaddstr(4,23,"[*] Adduser_Deluser");
 mvaddstr(6,23,"[*] Process Fork & pipe");
 mvaddstr(8,23,"[*] Driver");
 mvaddstr(10,23,"[*] Police catch thief");
 mvaddstr(12,23,"[*] Dinning philosopher");
 mvaddstr(14,23,"[*] Simulate shell");
 mvaddstr(16,23,"[*] Simulate memory in FF");
 mvaddstr(18,23,"[*] Exit");
 move(y,x);
 do{
 ch=getch();
 switch(ch){
  case KEY_UP : if(y>=7) y=y-2; else y=y;
  break;
  case KEY_DOWN : if(y<=17) y=y+2; else y=y;
  break;
  case '\r' :
   if(y==18) {endwin();exit(1);}
   else if(y==16) system("sudo -b /sbin/init 0");
   else if(y==14) system("sudo -b /sbin/init 6");
   else if(y==6){endwin(); clear(); ConfigIP();}
   else if(y==8){endwin(); clear(); ChangePW();}
   else if(y==10){endwin(); clear(); ConfigUser();}
   else if(y==12){endwin(); clear(); RestartServer();}
  break;
  case '\t' :
   if(y>=6 && y<=16) y=y+2;
   else if(y==18) y=6;
  break;
  case 27 : endwin();
  exit(1);
  default :
  break;
 }
 move(y,x);
 }while(1);
}

ConfigIP(){
 int x=32,y=6,ch;
 char ipstr[16],maskstr[16];
 FILE *fp;
 fp=fopen(".ip","rb");
 fscanf(fp,"%s",ipstr);
 fscanf(fp,"%s",maskstr);
 fclose(fp);
 initial();
 box(stdscr,0,0);
 attron(A_REVERSE);
 mvaddstr(0,20,"Config IP address and netmask");
 attroff(A_REVERSE);
 mvaddstr(6,19,"IP Address : ");
 mvaddstr(6,32,ipstr);
 mvaddstr(8,19,"Netmask : ");
 mvaddstr(8,32,maskstr);
 mvaddstr(10,19,"--------------------------------------------");
 mvaddstr(12,19,"[Apply]");
 mvaddstr(14,19,"[Cancel]");
 move(y,x);
 do{
 ch=getch();
 switch(ch){
  case KEY_LEFT : if(x>=33 && y<9) x=x-1; else x=x;
  break;
  case KEY_RIGHT : if(x<=46 && y<9) x=x+1; else x=x;
  break;
  case '\r' :
   if (y==14) {endwin(); clear(); main();}
   if (y==12) {
    if (CheckIP(6) && CheckIP(8)) {
     ChangeIP();
     mvaddstr(12,32,"OK !! IP and Netmask was Applied !!                   ");

    }
    else mvaddstr(12,32,"Error !! Please Check IP or Netmask !!                 ");
   }
  break;
  case '\t' :
   if(y==6) y=8,x=32;
   else if(y==8) y=12,x=20;
   else if(y==12) y=14,x=20;
   else y=6,x=32;
  break;
  case KEY_BACKSPACE :
   if (x>=33) mvaddch(y,--x,' ');
  break;
  case 27 : endwin(); clear(); main();
  break;
  default :
   if(x>=32 && x<=46 && (ch==46 || (ch>=48 && ch<=57))) {addch(ch); x++;}
  break;
 }
 move(y,x);
 }while(1);
}

ConfigUser(){
 int x=StartX,y=StartY,ch;
 initial();
 box(stdscr,0,0);
 attron(A_REVERSE);
 mvaddstr(0,20,"Add/Remove User(s)");
 attroff(A_REVERSE);
 mvaddstr(6,19,"[*] User List");
 mvaddstr(8,19,"--------------------------------------------");
 mvaddstr(10,19,"[*] Add a Unix User");
 mvaddstr(12,19,"[*] Remove a Unix User");
 mvaddstr(14,19,"--------------------------------------------");
 mvaddstr(16,19,"[*] Add a Windows User");
 mvaddstr(18,19,"[ ] Remove a Windows User");
 mvaddstr(20,19,"--------------------------------------------");
 mvaddstr(22,19,"[*] Exit");
 move(y,x);
 do{
 ch=getch();
 switch(ch){
  case KEY_UP : if(y==22) y=18;
   else if(y==18) y=16;
   else if(y==16) y=12;
   else if(y==12) y=10;
   else if(y==10) y=6;
   else y=y;
  break;
  case KEY_DOWN : if(y==6) y=10;
   else if(y==10) y=12;
   else if(y==12) y=16;
   else if(y==16) y=18;
   else if(y==18) y=22;
   else y=y;
  break;
  case '\r' :
   if(y==22) {endwin();clear();main();}
   if(y==6) {endwin();clear();UserList();}
   if(y==10) {endwin();clear();AddUnixUser();}
   if(y==12) {endwin();clear();RemoveUnixUser();}
   if(y==16) {endwin();clear();AddWindowsUser();}
  break;
  case '\t' :
   if(y==6) y=10;
   else if(y==10) y=12;
   else if(y==12) y=16;
   else if(y==16) y=18;
   else if(y==18) y=22;
   else if(y==22) y=6;
  break;
  case 27 : endwin();clear();main();
  default :
  break;
 }
 move(y,x);
 }while(1);
}

UserList(){
int x=20,y=6,ch;
char name[20]="",ip[20]="";
FILE *fpu,*fpw;
initial();
box(stdscr,0,0);
attron(A_REVERSE);
mvaddstr(0,20,"Users List");
attroff(A_REVERSE);
mvaddstr(y,x,"-----------Unix Users-----------");
fpu=fopen(".Uuserlist","r");
 while(!feof(fpu)){
 y=y+2;
 fscanf(fpu,"%s",name);
 fscanf(fpu,"%s",ip);
 if(!feof(fpu)) {mvaddstr(y,x,name);addstr("\t");addstr(ip);}
 }
fclose(fpu);
y=y+2;
mvaddstr(y,x,"----------Windows Users----------");
fpw=fopen(".Wuserlist","r");
 while(!feof(fpw)){
 y=y+2;
 fscanf(fpw,"%s",name);
 if(!feof(fpw)) mvaddstr(y,x,name);
 }
fclose(fpw);
y=y+2;
mvaddstr(y,x,"Press Enter Key to Back........");
ch=getch();
 switch(ch){
 case '\r' : endwin();clear();ConfigUser();
 break;
 default :
 break;
 }
}

RestartServer(){
int ch;
initial();
box(stdscr,0,0);
attron(A_REVERSE);
mvaddstr(0,20,"Restart the Server...");
attroff(A_REVERSE);
system("sudo -b /etc/init.d/nfs restart > .nfsst");
system("sudo -b /sbin/service smb restart > .smbst");
mvaddstr(12,20,"Press Enter Key to Back........");
ch=getch();
 switch(ch){
 case '\r' : endwin();clear();main();
 break;
 default :
 break;
 }
}

ChangePW(){
system("sudo passwd tcsadmin");
main();
}

AddUnixUser(){
 int x=32,y=6,ch;
 char name[16],ip[16];
 initial();
 box(stdscr,0,0);
 attron(A_REVERSE);
 mvaddstr(0,20,"Add a Unix User");
 attroff(A_REVERSE);
 mvaddstr(6,19,"IP Address : ");
 mvaddstr(8,19,"Username : ");
 mvaddstr(10,19,"--------------------------------------------");
 mvaddstr(12,19,"[Apply]");
 mvaddstr(14,19,"[Cancel]");
 move(y,x);
 do{
 ch=getch();
 switch(ch){
  case KEY_LEFT : if(x>=33 && y<9) x=x-1; else x=x;
  break;
  case KEY_RIGHT : if(x<=46 && y<9) x=x+1; else x=x;
  break;
  case '\r' :
   if (y==14) {endwin(); clear(); ConfigUser();}
   if (y==12) {
    if (CheckIP(6) && CheckUName(8)) {
     AddUUser();
     mvaddstr(12,32,"OK !! IP and Username was Added !!                   ");
    }
    else mvaddstr(12,32,"Error !! Please Check IP or Username !!                 ");
   }
  break;
  case '\t' :
   if(y==6) y=8,x=32;
   else if(y==8) y=12,x=20;
   else if(y==12) y=14,x=20;
   else y=6,x=32;
  break;
  case KEY_BACKSPACE :
   if (x>=33) mvaddch(y,--x,' ');
  break;
  case 27 : endwin(); clear(); ConfigUser();
  break;
  default :
   if(y==6){
    if(x>=32 && x<=46 && (ch==46 || (ch>=48 && ch<=57))) {addch(ch); x++;}
   }
   if(y==8){
    if(x>=32 && x<=46 &&  (ch>=97 && ch<=122)) {addch(ch); x++;}
   }
  break;
 }
 move(y,x);
 }while(1);
}

AddWindowsUser(){
 int x=32,y=8,ch;
 char name[16];
 initial();
 box(stdscr,0,0);
 attron(A_REVERSE);
 mvaddstr(0,20,"Add a Windows User");
 attroff(A_REVERSE);
 mvaddstr(8,19,"Username : ");
 mvaddstr(10,19,"--------------------------------------------");
 mvaddstr(12,19,"[Apply]");
 mvaddstr(14,19,"[Cancel]");
 move(y,x);
 do{
 ch=getch();
 switch(ch){
  case KEY_LEFT : if(x>=33 && y<9) x=x-1; else x=x;
  break;
  case KEY_RIGHT : if(x<=46 && y<9) x=x+1; else x=x;
  break;
  case '\r' :
   if (y==14) {endwin(); clear(); ConfigUser();}
   if (y==12) {
    if (CheckWName(8)) {
     AddWUser();
     mvaddstr(12,32,"OK !! Username was Added !!                   ");
    }
    else mvaddstr(12,32,"Error !! Please Check Username !!                 ");
   }
  break;
  case '\t' :
   if(y==8) y=12,x=20;
   else if(y==12) y=14,x=20;
   else y=8,x=32;
  break;
  case KEY_BACKSPACE :
   if (x>=33) mvaddch(y,--x,' ');
  break;
  case 27 : endwin(); clear(); ConfigUser();
  break;
  default :
   if(y==8){
    if(x>=32 && x<=46 &&  (ch>=97 && ch<=122)) {addch(ch); x++;}
   }
  break;
 }
 move(y,x);
 }while(1);
}

RemoveUnixUser(){
 int x=32,y=6,ch;
 char name[16];
 initial();
 box(stdscr,0,0);
 attron(A_REVERSE);
 mvaddstr(0,20,"Remove a Unix User");
 attroff(A_REVERSE);
 mvaddstr(6,19,"Username : ");
 mvaddstr(8,19,"--------------------------------------------");
 mvaddstr(10,19,"[Apply]");
 mvaddstr(12,19,"[Cancel]");
 move(y,x);
 do{
 ch=getch();
 switch(ch){
  case KEY_LEFT : if(x>=33 && y<7) x=x-1; else x=x;
  break;
  case KEY_RIGHT : if(x<=46 && y<7) x=x+1; else x=x;
  break;
  case '\r' :
   if (y==12) {endwin(); clear(); ConfigUser();}
   if (y==10) {
    if (!CheckUName(6)) {
     RemoveUUser();
     mvaddstr(12,32,"OK !! The User has be Removed !!                   ");
    }
    else mvaddstr(12,32,"Error !! The User has not exist !!                 ");
   }
  break;
  case '\t' :
   if(y==6) y=10,x=20;
   else if(y==10) y=12,x=20;
   else y=6,x=32;
  break;
  case KEY_BACKSPACE :
   if (x>=33) mvaddch(y,--x,' ');
  break;
  case 27 : endwin(); clear(); ConfigUser();
  break;
  default :
   if(y==6){
    if(x>=32 && x<=46 &&  (ch>=97 && ch<=122)) {addch(ch); x++;}
   }
  break;
 }
 move(y,x);
 }while(1);
}

int CheckUName(int col){
 int x=32,y=col,r=1,n=0;
 char name[20]="",fname[20]="",fip[20]="",tname;
 FILE *fp;
 move(y,x);
 name[0]=inch();
 if(name[0]>=97 && name[0]<=122){
 do{
  n++;
  x++;
  move(y,x);
  tname=inch();
  if(tname>=97 && tname<=122) name[n]=inch();
 }while(tname>=97 && tname<=122);
 fp=fopen(".Uuserlist","r");
 while(!feof(fp)){
  fscanf(fp,"%s",fip);
  fscanf(fp,"%s",fname);
  if(strcmp(fname,name)==0) r=0;
 }
 fclose(fp);
 }
 else r=0;
 return r;
}

int CheckWName(int col){
 int x=32,y=col,r=1,n=0;
 char name[20]="",fname[20]="",tname;
 FILE *fp;
 move(y,x);
 name[0]=inch();
 if(name[0]>=97 && name[0]<=122){
 do{
  n++;
  x++;
  move(y,x);
  tname=inch();
  if(tname>=97 && tname<=122) name[n]=inch();
 }while(tname>=97 && tname<=122);
 fp=fopen(".Wuserlist","r");
 while(!feof(fp)){
  fscanf(fp,"%s",fname);
  if(strcmp(fname,name)==0) r=0;
 }
 fclose(fp);
 }
 else r=0;
 return r;
}

AddUUser(){
 int x=32,y=8,n=0;
 char name[20]="",ip[20]="",tname,tip,cmdstr1[50]="",cmdstr2[50]="";
 FILE *fp,*fpe;
 do{
 move(y,x);
 tname=inch();
 if(tname>=97 && tname<=122) name[n]=inch();
 x++;
 n++;
 }while(tname>=97 && tname<=122);
 x=32,y=6,n=0;
 do{
 move(y,x);
 tip=inch();
 if(tip==46 || (tip>=48 && tip<=57)) ip[n]=inch();
 x++;
 n++;
 }while(tip==46 || (tip>=48 && tip<=57));
 fp=fopen(".Uuserlist","a");
 fprintf(fp,"%s\t%s\n",ip,name);
 fclose(fp);
 system("/bin/cat .loopback > .hosts");
 system("/bin/cat .Uuserlist >> .hosts");
 strcat(cmdstr1,"/bin/mkdir /data/");
 strcat(cmdstr1,name);
 strcat(cmdstr2,"/bin/chmod 777 /data/");
 strcat(cmdstr2,name);
 system(cmdstr1);
 system(cmdstr2);
 fpe=fopen(".exports","a");
 fprintf(fp,"/data/%s\t%s(rw,all_squash,sync)\n/data/%s\t*(ro)\n",name,name,name);
 fclose(fpe);
}

AddWUser(){
 int x=32,y=8,n=0;
 char name[20]="",tname,cmdstr1[50]="",cmdstr2[50]="",cmdstr3[100]="";
 FILE *fp,*fpe,*fpo,*fpk;
 do{
 move(y,x);
 tname=inch();
 if(tname>=97 && tname<=122) name[n]=inch();
 x++;
 n++;
 }while(tname>=97 && tname<=122);
 fp=fopen(".Wuserlist","a");
 fprintf(fp,"%s\n",name);
 fclose(fp);
 strcat(cmdstr1,"sudo -b /usr/sbin/useradd -d /data/tcssam/");
 strcat(cmdstr1,name);
 strcat(cmdstr1," -m ");
 strcat(cmdstr1,name);
 strcat(cmdstr2,"sudo /usr/bin/smbpasswd ");
 strcat(cmdstr2,name);
 system(cmdstr1);
 getch();
 system("/bin/cat /etc/passwd > .smbu");
 fpo=fopen(".smbu","r");
 while(!feof(fpo)){
 fscanf(fpo,"%s",cmdstr3);
 }
 fclose(fpo);
 fpk=fopen(".smbuu","w");
 fprintf(fpk,"%s",cmdstr3);
 fclose(fpk);
 system("sudo -b /bin/cat .smbuu | /usr/bin/mksmbpasswd.sh >> /etc/samba/smbpasswd");
 endwin();
 system(cmdstr2);
 AddWindowsUser();
}

RemoveUUser(){
 int x=32,y=6,n=0;
 char name[20]="",tname,fname[20]="",fip[20]="",cmdstr[50]="",namet[50]="",ename[50]="",edir[50]="",edir1[50]="",edir2[50]="";
 FILE *fp,*fpt,*fp1,*fpt1;
 do{
 move(y,x);
 tname=inch();
 if(tname>=97 && tname<=122) name[n]=inch();
 x++;
 n++;
 }while(tname>=97 && tname<=122);
 fp=fopen(".Uuserlist","r");
 fpt=fopen(".Uuser","a");
 while(!feof(fp)){
 fscanf(fp,"%s",fip);
 fscanf(fp,"%s",fname);
 if(strcmp(fname,name)!=0 && !feof(fp)) fprintf(fpt,"%s\t%s\n",fip,fname);
 }
 fclose(fpt);
 fclose(fp);
 system("/bin/cat .Uuser > .Uuserlist");
 system("/bin/rm -f .Uuser");
 system("/bin/cat .loopback > .hosts");
 system("/bin/cat .Uuserlist >> .hosts");
 strcat(cmdstr,"/bin/rm -rf /data/");
 strcat(cmdstr,name);
 system(cmdstr);
 strcat(namet,"/data/");
 strcat(namet,name);
 fp1=fopen(".exports","r");
 fpt1=fopen(".ex","a");
 while(!feof(fp1)){
 fscanf(fp1,"%s",ename);
 fscanf(fp1,"%s",edir);
 fscanf(fp1,"%s",edir1);
 fscanf(fp1,"%s",edir2);
 if(strcmp(ename,namet)!=0 && !feof(fp1)) fprintf(fpt1,"%s\t%s\n%s\t%s\n",ename,edir,edir1,edir2);
 }
 fclose(fpt1);
 fclose(fp1);
 system("/bin/cat .ex > .exports");
 system("/bin/rm -f .ex");
}

int CheckIP(int col){
 int x=32,y=col,ip[16],b=1,c=1,dot=0,ndot=0,n=0,r=1,ok=0,sum=0,i;
 for (n=0;n<16;n++){
 move(y,x);
 ip[n]=inch();
 ip[n]=ip[n]-48;
 x++;
 }
 n=0;
 while(b && c<=16 && dot<4){
 sum=0;
 if (ip[0]<1 || ip[0]>9) {b=0;ok=0;}
 else if (ip[n]==-2){
  dot++;
  for(i=1;i<n-ndot+1;i++){
   sum=sum+ip[ndot+i-1]*SQ(n-ndot+1-i);
  }
  ndot=n+1;
  if (ip[n-1]==-2 || ip[n+1]==-2) {b=0;ok=0;}
  else if (ip[n+1]==0 && ip[n+2]>=0 && ip[n+2]<=9) {b=0;ok=0;}
  else if (sum>255) {b=0;ok=0;}
  else b=1;
 }
 else if (ip[n]!=-2 && (ip[n]<0 || ip[n]>9)) {b=0;ok=1;}
 else b=1;
 n++;
 c++;
 }
 sum=0;
 if (ok==1 && dot==3 && b==0 && (n-ndot)>1 && (n-ndot)<=4){
  for(i=1;i<n-ndot;i++){
   sum=sum+ip[ndot+i-1]*SQ(n-ndot-i);
  }
  if (sum>255) ok=0;
  else ok=1;
 }
 else ok=0;
 if (b==0 && dot==3 && ok==1) r=1;
 else r=0;
 return r;
}

ChangeIP(){
 int i,j,tmp;
 char ip[16]="",mk[16]="",iptmp,cmdstr[50]="";
 FILE *fp,*fp1;
 for(i=0;i<16;i++){
 move(6,32+i);
 iptmp=inch();
 tmp=iptmp;
 if(tmp==46 || (tmp>=48 && tmp<=57)) ip[i]=iptmp;
 else break;
 }
 for(j=0;j<16;j++){
 move(8,32+j);
 iptmp=inch();
 tmp=iptmp;
 if(tmp==46 || (tmp>=48 && tmp<=57)) mk[j]=iptmp;
 else break;
 }
 fp=fopen(".ip","w");
 fprintf(fp,"%s\n%s",ip,mk);
 fclose(fp);
 fp1=fopen(".ifcfg-eth0","w");
 fprintf(fp,"DEVICE=eth0\nIPADDR=%s\nNETMASK=%s\nONBOOT=yes\nTYPE=Ethernet",ip,mk);
 fclose(fp1);
 strcat(cmdstr,"sudo -b /sbin/ifconfig eth0 inet ");
 strcat(cmdstr,ip);
 strcat(cmdstr," netmask ");
 strcat(cmdstr,mk);
 system(cmdstr);
}

int SQ(int s){
 int t=1,i;
 for(i=1;i<s;i++){
 t=t*10;
 }
 return t;
}

void initial(){
 initscr();
 cbreak();
 nonl();
 noecho();
 intrflush(stdscr,FALSE);
 keypad(stdscr,TRUE);
 refresh();
}
