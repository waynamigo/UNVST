#include <iostream>
using namespace std;
class BinaryTree;
class BinTreeNode
{
    friend  BinaryTree;
    BinTreeNode *leftChild,*rightChild,*parent;
    char data;
    int layer;
public:
    BinTreeNode()
    {
        leftChild =NULL;
        rightChild =NULL;
        parent=NULL;
        layer=0;
    }
    BinTreeNode(char x, BinTreeNode *left = NULL, BinTreeNode *right = NULL){
        data=x;
        leftChild=left;
        rightChild=right;
        parent=NULL;
        layer=0;
    }
    ~BinTreeNode(){ }
};
//class Queue
//{
//    BinTreeNode **element;
//    int head,tail,size;
//public:
//    Queue(){size =30;head=0;tail=0;element=new BinTreeNode* [size];}
//    ~Queue(){}
//    void enQue(BinTreeNode *p){
//        element[tail]=p;
//        tail=(tail+1)%size;
//    }
//    void deQue(BinTreeNode *&p){
//        p=element[head];
//        head=(head+1)%size;
//    }
//    bool isempty(){return head==tail;}
//
//};
class BinaryTree
{
public:
    BinaryTree ()
    {
        EndTag ='#';
        root =NULL;
        current=NULL;
        leafnum=0;
        nodenum=0;
    }
    void CreateBinTree( )
    {
        CreateBinTree(root);
    }
    void getheight()
    {
        cout<<"Height="<<height(root)<<"."<<endl;
    }
    void getLeaves()
    {
        leaves(root);
        cout<<"Leaf="<<leafnum<<"."<<endl;
    }
    void getNodes()
    {
        nodes(root);
        cout<<"Nodes="<<nodenum<<"."<<endl;
    }
    void inOrder( )
    {
        inOrder(root);
    }
    void preOrder()
    {
        preOrder(root);
    }
    void posOrder()
    {
        posOrder(root);
    }
    int getcount(char F)
    {
        return getcount(root,F);
    }

    void destroy(){
        destroy(root);
    }
    void prePrint(){
        prePrint(root);
    }
//    void prePrint(){
//        Queue q;
//        BinTreeNode *p;
//        p=root;
//        q.enQue(p);
//        int cnt=0;
//        while(!q.isempty()){
//            q.deQue(p);
//            for(int i=0;i<cnt;i++){
//                cout<<" ";
//            }
//            cout<<p->data<<endl;
//            if(p->leftChild) q.enQue(p->leftChild);
//            if(p->rightChild) q.enQue(p->rightChild);
//        }
//    }
private:
    BinTreeNode *root,*current;
    char EndTag;
    int nodenum,leafnum;
    void CreateBinTree(BinTreeNode* & subTree)
    {
        char item;
        cin>> item;
        if (item!=EndTag)
        {
            subTree=new BinTreeNode(item);
            CreateBinTree(subTree->leftChild);
            CreateBinTree(subTree->rightChild);
        }
    }
    void inOrder(BinTreeNode *subTree)
    {
        if (subTree!= NULL)
        {
            inOrder(subTree->leftChild);
            cout<<subTree->data<<" ";
            inOrder(subTree->rightChild);
        }
    }
    void posOrder(BinTreeNode *subTree)
    {
        if (subTree!= NULL)
        {
            posOrder(subTree->leftChild);
            posOrder(subTree->rightChild);
            cout<<subTree->data<<" ";
        }
    }
    void preOrder(BinTreeNode *subTree)
    {
        if (subTree!= NULL)
        {
            cout<<subTree->data<<" ";
            preOrder(subTree->leftChild);
            preOrder(subTree->rightChild);
        }
    }
    int height(BinTreeNode *subTree){
        if(subTree==NULL) return 0;
        else return 1+max(height(subTree->leftChild),height(subTree->rightChild));
    }
    void leaves(BinTreeNode *subTree){
        if(subTree!=NULL){
            if(subTree->leftChild==NULL&&subTree->rightChild==NULL){
                leafnum++;
            }
            else{
                leaves(subTree->leftChild);
                leaves(subTree->rightChild);
            }
        }
    }
    void nodes(BinTreeNode *subTree){
        if(subTree!=NULL){
            if(isupper(subTree->data)){
                nodenum++;
            }
            nodes(subTree->leftChild);
            nodes(subTree->rightChild);
        }
    }
    int getcount(BinTreeNode *subTree,char f){
        if(subTree==NULL) return 0;
        else{
            if(subTree->data==f){
                return 1+getcount(subTree->leftChild,f)+getcount(subTree->rightChild,f);
            }
            else return 0;
        }
    }
    int depth(BinTreeNode *T,char x,int l)
    {
        int i=-1;
        if(T!=NULL)
        {
            if(T->data==x)
                return l;
            if((i=depth(T->leftChild,x,l+1))!=-1)
                return i;
            if((i=depth(T->rightChild,x,l+1))!=-1)
                return i;
        }
        return i;
    }
    void prePrint(BinTreeNode *subTree)
    {
        if (subTree!= NULL)
        {
            int DEEP=0;
            int n = depth(root,subTree->data,DEEP);
            while(n--){
                cout<<"  ";
            }
            cout<<subTree->data<<endl;
            prePrint(subTree->leftChild);
            prePrint(subTree->rightChild);
        }
    }
    void destroy(BinTreeNode *subTree){
        if(subTree!=NULL){
            destroy(subTree->leftChild);
            destroy(subTree->rightChild);
            delete subTree;
        }
    }
};
int main()
{
    int cnt=0;
    char c='X',FIND;
    BinaryTree A;
    while(c!='P'){
        cin>>c;
        switch(c){
            case 'C':
                A.CreateBinTree();
                cout<<"Created success!"<<endl;
                break;
            case 'H':
                A.getheight();
                break;
            case 'L':
                A.getLeaves();
                break;
            case 'N':
                A.getNodes();
                break;
            case '1':
                cout<<"Preorder is:";A.preOrder();cout<<'.'<<endl;
                break;
            case '2':
                cout<<"Inorder is:";A.inOrder();cout<<'.'<<endl;
                break;
            case '3':
                cout<<"Postorder is:";A.posOrder();cout<<'.'<<endl;
                break;
            case 'F':
                cin>>FIND;
                cout<<"The count of "<<FIND<<" is "<<A.getcount(FIND)<<'.'<<endl;
                break;
            case 'P':
                cout<<"The tree is:"<<endl;
                A.prePrint();
                break;
        }
    }
    A.destroy();
    return 0;
}













