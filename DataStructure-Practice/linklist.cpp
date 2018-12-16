#include <iostream>
#include <stdio.h>
#include<algorithm>
using namespace std;
class Listot; //前视定义,否则友元无法定义
class LinkNodeot
{
 friend  Listot; //链表结点类的定义
 private:
   LinkNodeot *link;
   int data;
 public:
   LinkNodeot (LinkNodeot *ptr = NULL)    {link=ptr;}
   LinkNodeot(const int & item, LinkNodeot *ptr = NULL){  data=item;link=ptr;}
   ~LinkNodeot(){};
};

class Listot
{//单链表类的定义
private:
    LinkNodeot *first; //指向首结点的指针
    int getlen(){
        int cnt=0;
        LinkNodeot *p=first->link;
        while(p){
            cnt++;
            p=p->link;
        }
        return cnt;
    }
public:
     Listot () { first = new LinkNodeot ();}   // 带头结点
	 ~Listot (){ }         //析构函数
    void input(int  endTag);
    void output();
    void insert(int i ,int a);
    int find_id(int x);
    void remove(int x);
    void intersec(Listot *A,Listot *B);
    void union1(Listot *A,Listot *B);
    void sortdata();
    void reverse();
};
void Listot::reverse(){
    LinkNodeot *p,*q,*pr;
    p= first->link;
    pr=NULL;
    q=NULL;
    first->link=NULL;
    while(p){
        pr=p->link;
        p->link=q;
        q=p;
        p=pr;
    }
    first->link = q;

}
void Listot::input(int endTag){
    LinkNodeot *rear,*e ;
    rear =first;
    int i;
    cin>>i;
    while(i!=endTag){
        e=new LinkNodeot(i);
        rear->link=e;
        rear=e;
        cin>>i;
    }
    rear->link =NULL;
}
void Listot::output(){
    LinkNodeot *e;
    e = first;
    while(e->link){
        e=e->link;
        cout<<' '<<e->data;
    }
}
void Listot::insert(int i,int a){
    LinkNodeot *p,*e;
    p= first;
    int cnt=0;
    while(cnt<i-1){
        p=p->link;
        cnt++;
    }
    e=new LinkNodeot(a);
    e->link=p->link;
    p->link=e;
}
int Listot::find_id(int x){
    LinkNodeot *p=first->link;
    int i,flag=0;
    for(i=1;p!=NULL;i++)
    {
        if(p->data==x){
            flag=1;
            break;
        }
        p=p->link;
    }
    if(flag==1)    return i;
    else return 0;
}
void Listot::remove(int x){
    LinkNodeot *p,*e;
    p=first;
    while(p->link){
        if(p->link->data==x){
            e=p->link;
            p->link=e->link;
            delete e;
        }
        p=p->link;
    }
}
void Listot::intersec(Listot *A,Listot *B){
    LinkNodeot *a,*b;
    a=A->first->link;
    cout<<"A cross B is";
    while(a){
        int c = B->find_id(a->data);
        if(c!=0){
            cout<<' '<<a->data;
        }
        a=a->link;
    }
    cout<<endl;
}
void Listot::union1(Listot *A,Listot *B){
    LinkNodeot *a,*b,*p,*s;
    a=A->first->link;
    p=first;
    while(a){
        s=new LinkNodeot(a->data);
        p->link=s;
        p=p->link;
        a=a->link;
    }
    a=A->first->link;
    b=B->first->link;
    int c;
    while(b){
        c = A->find_id(b->data);
        if(c==0){
            s=new LinkNodeot(b->data);
            p->link=s;
            p=p->link;
        }
        b=b->link;
    }
}
void Listot::sortdata(){
    int len=getlen()+1;
    int *copyarr=new int[len];
    LinkNodeot *p=first->link;
    for(int i=0;i<len-1;i++){
        copyarr[i]=p->data;
        p=p->link;
    }
   // for(int i=0;i<len-1;i++) cout<<copyarr[i]<< ' ';
    p=first->link;
    sort(copyarr,copyarr+len-1);
    for(int i=0;i<len-1;i++){
        p->data=copyarr[i];
        p=p->link;
    }
}
int main()7-
{
    Listot A;
    int la;
    cin>>la;
    int i,a,b,c;
    A.input(la);
    cout<<"A is created as:";A.output();cout<<endl;
//    cin>>i>>a;
//    A.insert(i,a);
//    cout<<"After inserted A is";A.output();cout<<' '<<endl;
//    cin>>b;
//    A.remove(b);
//    cout<<"After deleted A is";A.output();cout<<endl;
//    cin>>b;
//    if(A.find_id(b)==0)
//        cout<<b<<" is not found"<<endl;
//    else
//        cout<<b<<" is located at index of "<<A.find_id(b)<<endl;
//    cin>>b;
//    if(A.find_id(b)==0)
//        cout<<b<<" is not found"<<endl;
//    else
//        cout<<b<<" is located at index of "<<A.find_id(b)<<endl;
//    int lb;
//    cin>>lb;
//    Listot B,C,D;
//    B.input(lb);
//    cout<<"B is created as:";B.output();cout<<endl;
//    C.intersec(&A,&B);
//    D.union1(&A,&B);
//    cout<<"A union B is";D.output();cout<<endl;
//    D.sortdata();
//    cout<<"A union B in sequence is";D.output();cout<<endl;
    A.reverse();
    A.output();
}
















