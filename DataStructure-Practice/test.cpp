#include<iostream>
#include<cstring>
using namespace std;
typedef struct Trituple{
    int row,col;
    int value;

}triple;
class Matrix{
    triple  * triples;
    int sumterms;
    int rows,cols;
public:
    Matrix(int rs,int cs,int st):rows(rs),cols(cs),sumterms(st){ triples = new triple[st];}
    void destroy(){
        delete triples;
    }
    void input(){
        int row,col,value;
        int i=0;
        while(i<sumterms){
            cin>>row>>col>>value;
            triples[i].row=row;
            triples[i].col=col;
            triples[i].value=value;
            i++;
        }
    }
    void output(){
        for(int i =0;i<sumterms;i++){
            cout<<triples[i].row<<triples[i].col<<triples[i].value<<endl;
        }
    }
    void sparse(){
        int pr,ps,pv;
        for(int i =0; i<sumterms;i++){
            int *rowsize= new int[cols];
            int *rowstart= new int[cols];
            Matrix R=new Matrix(cols,rows,sumterms);
            memset(rowsize,0,sizeof(rowsize));
            for(int i;i<sumterms;i++){
                rowsize[triples[i].col]++;
            }
        }
    }

};
int main(){
    int r,s,t;
    cin>>r>>s>>t;
    Matrix matrix = new Matrix(r,s,t);
    matrix.input();
    matrix.output();
}
