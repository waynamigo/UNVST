#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
class Graph
{
public:
	Graph(int sz){
		size=sz;
		int i=0;
		Ver	= new char[size];
		Edge= new int*[size];
		for(int i=0;i<size;i++){
            Edge[i]=new int[size];
            for(int j=0;j<size;j++){
                Edge[i][j]=0;
            }
		}
        while(sz--){
            cin>>Ver[i++];
        }
	}
	~Graph(){
		delete Ver;
		delete Edge;
	}

	void input(int n){
		int i,j,w;
		while(n--){
			cin>>i>>j>>w;
			 	Edge[i][j]=w;
			Edge[j][i]=w;
		}
	}
	void output(){
		for (int i = 0; i < size; ++i)
		{
			for (int j = i; j <size; ++j)
			{
				if(Edge[i][j])
					cout<<Ver[i]<<'-'<<Ver[j]<<':'<<Edge[i][j]<<endl;
			}
		}
	}
	void printDFS(){
		bool visited[size];
		memset(visited,false,sizeof(visited));
		int i=0;
		DFS(i,visited);
	}
	void printBFS(){
		bool visited[size];
		memset(visited,false,size);
		int i=0;
		BFS(i,visited);
	}
private:
	int size;
	char *Ver;
	int **Edge;

	
	void DFS(int k,bool *visited){
		cout<<Ver[k]<<' ';
        visited[k]=true;
        for(int i=0;i<size;++i){
            if(Edge[k][i]&&visited[i]==false){
                DFS(i,visited);
            }
        }
	}

	void BFS(int k,bool *visited){
		cout<<Ver[k]<<' ';
        visited[k]=true;
        queue<int> q;
        q.push(k);
		while(!q.empty()){
            int f=q.front();
            q.pop();
            for(int i=0;i<size;i++){
                if(Edge[f][i]&&visited[i]==false){
                    visited[i]=true;
                    cout<<Ver[i]<<' ';
                    q.push(i);
                }
            }
        }
	}
};
int main()
{
	int sz,n;
	cin>>sz;
	Graph graph(sz);
	cin>>n;
	graph.input(n);

	cout<<"DFS:";graph.printDFS();cout<<endl;
	cout<<"BFS:";graph.printBFS();cout<<endl;
    cout<<"edges are:"<<endl;
    graph.output();
	return 0;
}
