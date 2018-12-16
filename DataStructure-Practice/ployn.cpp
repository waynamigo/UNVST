#include<iostream>
#include<cmath>
using namespace std;
class Ployn;
class Term{
    friend class Ployn;
    Term * next;
    int exp;
    int coef;
public:
    Term(int c,int e,Term *p=NULL){
            coef =c;
            exp  =e;
            next =p;
    }
};
class Ployn{
    Term *first;
    int maxexp;
public:
    Ployn(){
        first = new Term(0,-1);
        maxexp =20;
    }
    void drv(Ployn *n){
        Term *p,*rear,*e;
        p    = n->first->next;
        rear = first;
        while(p){
            e=new Term(p->coef*p->exp,p->exp-1);
            rear->next= e;
            rear = e;
            p=p->next;
        }
        rear->next=NULL;
    }
    int calculate(int x){
        Term *p;
        p=first->next;
        int result=0;
        while(p){
            result+=(int)(pow(x,p->exp))*(p->coef);
            p=p->next;
        }
        return result;
    }
    void plusP(Ployn *p1,Ployn *p2){
        Term *m,*n,*p,*e;
		p = first;
		m= p1->first->next;
        n= p2->first->next;
        while(m&&n){
            if(m->exp < n->exp){
                e= new Term(m->coef,m->exp);
                p->next=e;
                p=e;
                m=m->next;
            }
            else if(m->exp>n->exp){
                e= new Term(n->coef,n->exp);
                p->next=e;
                p=e;
                n=n->next;
            }
            else{
                e=new Term(m->coef+n->coef,m->exp);
                p->next =e;
                p=e;
                n=n->next;
                m=m->next;
            }
        }
        if(m!=NULL) p->next=m;
        if(n!=NULL) p->next=n;

    }
    void input(int n){
        Term *rear,*p ;
        rear = first;
        int c,e;
        while(n--){
            cin>>c>>e;
            p=new Term(c,e);
            rear->next=p;
            rear=p;
        }
        maxexp =rear->exp;
        rear->next =NULL;
    }
    void output(){
        Term *p;
        p=first->next;
        if(p->exp==0){
            if(p->coef!=0) cout<<p->coef;
            else p=p->next;
        }
        else{
            if(p->coef==1){
                if(p->exp==1) cout<<"x";
                else cout<<"x^"<<p->exp;
            }
            else{
                if(p->exp==1) cout<<p->coef<<"x";
                else cout<<p->coef<<"x^"<<p->exp;
            }

        }
        p=p->next;

        while(p){
            if(p->coef==0){
                p=p->next;
                continue;
            }
            else if(p->coef==1){
                if(p->exp==1) cout<<"+x";
                else cout<<"+x^"<<p->exp;
            }
            else{
                if(p->exp==1) cout<<"+"<<p->coef<<"x";
                else cout<<"+"<<p->coef<<"x^"<<p->exp;
            }
            p=p->next;
        }
        cout<<endl;
    }
};
int main()
{
    Ployn a,b,c,d;
    int n,x;
    cin>>n;
    a.input(n);
    cin>>n;
    b.input(n);

    cin>>x;
    c.plusP(&a,&b);
    d.drv(&a);
    cout<<"A(x)+B(x)=";c.output();
    cout<<"A'(x)=";d.output();

    cout<<"A("<<x<<")="<<a.calculate(x)<<endl;

}
