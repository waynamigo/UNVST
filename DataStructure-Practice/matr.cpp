#include<iostream>
using namespace std;
class Triple{
public:
    int row,col,value;
};
class Matrix{
    Triple *arr;
    int maxsize;
    int rows,cols,terms;
public:
    Matrix(int r,int c,int t){
        rows=r;
        cols=c;
        terms=t;
        arr = new Triple[terms];
    }
    void input(){
        int cnt=0;
        while(cnt<terms){
            cin>>arr[cnt].row>>arr[cnt].col>>arr[cnt].value;
            cnt++;
        }
    }
    void output(){
        for(int i=0;i<terms;i++){
            cout<<arr[i].row<<' '<<arr[i].col<<' '<<arr[i].value<<endl;
        }
    }
    Matrix trans(){
        Matrix transm(cols,rows,terms);
        int rowsize[cols]={0};
        int rowstart[cols]={0};
        for(int i=0;i<terms;i++){
            rowsize[arr[i].col]++;
        }
        for(int i=1;i<cols;i++){
            rowstart[i]=rowstart[i-1]+rowsize[i-1];
        }
        for(int i=0;i<terms;i++){
            int t=rowstart[arr[i].col];
            transm.arr[t].row=arr[i].col;
            transm.arr[t].col=arr[i].row;
            transm.arr[t].value=arr[i].value;
            rowstart[arr[i].col]++;
        }
        return transm;
    }
    void add(Matrix a){
        for(int i=0;i<a.terms;i++){
            a.arr[i]
        }
    }
};
int main(){
    int n,cnt=0;
    int row,col;
    cin>>row>>col>>n;
    Matrix m1=Matrix(row,col,n);
    m1.input();
    Matrix m=m1.trans();
    cout<<"The transformed matrix is:";m.output();cout<<endl;
//    cin>>row>>col>>n;
//    Matrix m2=Matrix(row,col,n);
//    m2.input();

}
