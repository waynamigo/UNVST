#include<iostream>
#include<string>
using namespace std;
char loss[20];
int index=0;
class Stack{
private:
    char *elements;
    int top;
    int maxsize;

public:
    Stack(int s=20){
        maxsize = s;
        top=-1;
        elements =  new char[maxsize];
    }
    void push(char c){
        top++;
        elements[top]=c;
    }
    void pop(){
        elements[top--];
    }
    void outputloss(){
        for(int i=top;i>=0;i--){
            switch(elements[i]){
                case '{':
                    cout<<'}';
                    break;
                case '[':
                    cout<<']';
                    break;
                case '(':
                    cout<<')';
                    break;
                case '<':
                    cout<<'>';
                    break;
            }
        }
    }
    int before(){return elements[top];}
    bool isempty(){
        if(top==-1) return true;
        else return false;
    }
};
int main()
{
    char s[20],c;
    int cnt=0,w=0;
    while(cin>>c){
        if(c=='#')break;
        else s[cnt++]=c;
    }
    Stack st(20);
    for(int i=0;i<cnt;i++){
        switch (s[i]){
            case '{':
                st.push(s[i]);
                break;
            case '}':
                if(st.before()=='{'){
                    st.pop();
                }
                else{
                    cout<<"The "<<i+1<<" character '"<<s[i]<<"' is wrong."<<endl;
                    return 0;
                }
                break;
            case '[':
                st.push(s[i]);
                break;
            case ']':
                if(st.before()=='['){
                    st.pop();
                }
                else{
                    cout<<"The "<<i+1<<" character '"<<s[i]<<"' is wrong."<<endl;
                    return 0;
                }
                break;
            case '(':
                st.push(s[i]);
                break;
            case ')':
                if(st.before()=='('){
                    st.pop();
                }
                else{
                    cout<<"The "<<i+1<<" character '"<<s[i]<<"' is wrong."<<endl;
                    return 0;
                }
                break;
            case '<':
                st.push(s[i]);
                break;
            case '>':
                if(st.before()=='<'){
                    st.pop();
                }
                else{
                    cout<<"The "<<i+1<<" character '"<<s[i]<<"' is wrong."<<endl;
                    return 0;
                }
                break;
        }
    }
    if(st.isempty()){cout<<"right."<<endl;}
    else{cout<<"loss of right character ";st.outputloss();cout<<'.'<<endl;}

    return 0;
}
