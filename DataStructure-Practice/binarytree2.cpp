#include <iostream>
using namespace std;
class TreeNode
{
public:
    char data;
    TreeNode *lchild;
    TreeNode *rchild;
    TreeNode()
    {
        lchild=NULL;
        rchild=NULL;
    }
    TreeNode(char d,)
    {
        data=d;
        lchild=NULL;
        rchild=NULL;
    }
};
class BinaryTree
{
public:
    TreeNode *root;
    BinaryTree()
    {
        root=NULL;
    }
    void createBinaryTree()
    {
        createBinaryTree(root);
    }
    void createBinaryTree(TreeNode *(&subTree))
    {
        char d;
        if(!cin.eof())
        {
            cin>>d;
            if(d!='@')
            {
                subTree = new TreeNode(d);
                createBinaryTree(subTree->lchild);
                createBinaryTree(subTree->rchild);
            }
        }
    }
    void print()
    {
        TreeNode *ltemp=root;
        TreeNode *rtemp=root;
        inOrder(root,ltemp,rtemp);
        cout<<'='<<calculate(root)<<endl;
    }
    void inOrder(TreeNode *subTree,TreeNode *(&ltemp),TreeNode *(&rtemp))
    {
        if(subTree!=NULL)
        {
            if((subTree->data=='*'||subTree->data=='/')&&countNodes(subTree->lchild)!=1)
            {
                cout<<'(';
                ltemp=subTree->lchild;
            }
            inOrder(subTree->lchild,ltemp,rtemp);
            cout<<subTree->data;
            subTree->flag=true;
            if(isAllPrinted(ltemp)&&countNodes(subTree)!=1)
            {
                cout<<')';
                ltemp=NULL;
            }
            if((subTree->data=='*'||subTree->data=='/')&&countNodes(subTree->rchild)!=1)
            {
                cout<<'(';
                rtemp=subTree->rchild;
            }
            inOrder(subTree->rchild,ltemp,rtemp);
            if(isAllPrinted(rtemp)&&countNodes(subTree)!=1)
            {
                cout<<')';
                rtemp=NULL;
            }
        }
    }
    int countNodes(TreeNode *subTree)
    {
        if(subTree!=NULL)
        {
            return countNodes(subTree->lchild)+countNodes(subTree->rchild)+1;
        }
        else
        {
            return 0;
        }
    }
    bool isAllPrinted(TreeNode *subTree)
    {
        if(subTree==NULL)return false;
        bool flag=true;
        printed(subTree,flag);
        return flag;
    }
    void printed(TreeNode *subTree,bool &flag)
    {
        if(subTree!=NULL)
        {
            if(subTree->flag==false)
            {
                flag=false;
            }
            printed(subTree->lchild,flag);
            printed(subTree->rchild,flag);
        }
    }
    int calculate(TreeNode *subTree)
    {
        if(subTree!=NULL)
        {
            if(subTree->data=='+')
            {
                return calculate(subTree->lchild)+calculate(subTree->rchild);
            }
            if(subTree->data=='-')
            {
                return calculate(subTree->lchild)-calculate(subTree->rchild);
            }
            if(subTree->data=='*')
            {
                return calculate(subTree->lchild)*calculate(subTree->rchild);
            }
            if(subTree->data=='/')
            {
                return calculate(subTree->lchild)/calculate(subTree->rchild);
            }
            else
            {
                return subTree->data-'0';
            }
        }
    }
};
int main()
{
    BinaryTree bt;
    bt.createBinaryTree();
    bt.print();
    return 0;
}
