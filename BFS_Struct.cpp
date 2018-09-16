#include<iostream>
#include<cmath>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<stack>
#include<algorithm>
#include<list>
#include<queue>
#include<cstring>
using namespace std;
char sNet[]={'#','A','B','C','D','E','F','G','H','I','J','K','L','M','N'};
struct Link{
	int linkId;
	int from;
	int to;
	double bandwidth;
	double residual_bw;
	double ratio;
	double weight;
};
struct Node{
	int nodeId;
	double CPU;
	double residual_cpu;
	double ratio;
};
struct shortestpath {
	int length;
	int next;
	double bw;
};
class SubstrateNetwork{
public:
	int nodes;
	int links;
	double totalCPU;
	double totalBW;
	map<int, Node> mapNodes;//这里根据nodeIndex进行存取
	map<int, Link> mapLinks;//这里根据linkIndex进行存取
	vector<vector<int>> matrix;//链路化成矩阵 便于floyd
	struct shortest_path* spath;

	void initSubstrateNetwork(char *filepath){//初始化
        FILE *fp;
        fp=fopen(filepath,"r");
        if(fscanf(fp,"%d%d",&nodes,&links)){
            vector<int> tempmatrix(nodes+1,-1);//老师代码的common.h
            matrix.resize(nodes+1,tempmatrix);//之前实现CAA时直接在main中用的vector重新读取，还是在初始化时直接存比较简洁
            totalBW=0;
            totalCPU=0;
            tempmatrix.clear();
            Node tempnode;
            Link templink;

            for(int i=0;i<nodes;i++){
                fscanf(fp,"%lf",&tempnode.CPU);
                tempnode.nodeId=i+1;//物理网络的节点按文件中从上往下 1 2 3 4 5 6号来存
                tempnode.residual_cpu=tempnode.CPU;
                tempnode.ratio=0;
                totalCPU+=tempnode.CPU;
                mapNodes.insert(pair<int,Node>(i,tempnode));
            }
            for(int i=0;i<links;i++){
                fscanf(fp,"%d %d %lf",&templink.from,&templink.to,&templink.bandwidth);
                templink.linkId=i+1;//边的序号从1开始  1 2 3 4 5
                templink.residual_bw=templink.bandwidth;
                templink.ratio=0;
                totalBW+=templink.bandwidth;
                mapLinks.insert(pair<int,Link>(i,templink));
                matrix.at(templink.from).at(templink.to)=templink.bandwidth;
                matrix[templink.to][templink.from]=templink.bandwidth;
            }
            fclose(fp);
        }
	}
	double getNodeLinkedBW(int nodeid){
        double sumbw=0;
        for(int i=0;i<mapLinks.size();i++){
            if(mapLinks[i].from==nodeid){
                sumbw+=mapLinks[i].bandwidth;
            }
        }
        return sumbw;
	}
	void sortNodes(){
        double cpu_bw[mapNodes.size()];
        for(int i=0;i<mapNodes.size();i++){
            cpu_bw[i] = mapNodes[i].CPU+getNodeLinkedBW(mapNodes[i].nodeId);
        }
        for(int i=0;i<mapNodes.size()-1;i++){
            for(int j=0;j<mapNodes.size()-1-i;j++){
                if(cpu_bw[j]<cpu_bw[j+1]){
                    double tmpcpu_bw=cpu_bw[j];
                    cpu_bw[j]=cpu_bw[j+1];
                    cpu_bw[j+1]=tmpcpu_bw;
                    Node node = mapNodes.at(j);
                    mapNodes.at(j)=mapNodes.at(j+1);
                    mapNodes.at(j+1)=node;
                }
            }
        }
	}
	void print(){
        for(int i=0;i<nodes;i++){
            cout<<sNet[mapNodes.at(i).nodeId]<<"  ";
//            cout<<mapNodes.at(i).nodeId<<"  ";
        }
	}
    Node getnodebyId(int id){
        int i;
        for( i=0;i<nodes;i++){
            if(mapNodes.at(i).nodeId==id){
                return mapNodes.at(i);
            }
        }
    }
    int first_vertex(int v){
        int i;
        if (v<0 || v>nodes)
            return -1;

        for (i = 0; i <=nodes; i++)
            if (matrix[v][i]!=0 && matrix[v][i]!=-1)
                return i;
        return -1;
    }
    int next_vertix(int v,int w){
        int i;
        if (v<0 || v>nodes || w<0 || w>nodes)
            return -1;
        for (i = w+1 ; i <= nodes; i++)
            if (matrix[v][i]!=0 && matrix[v][i]!=-1)
                return i;
        return -1;
    }
};
struct Request{
    int requestId;
    int nodes;
    int links;
    map<int,Node> mapNodes;
	map<int,Link> mapLinks;
//	double GetRevenue();//计算请求的收益=CPU收益+带宽收益
	vector<vector<int>> matrix;
	void initRequest(char *filepath){//初始化
        FILE *fp;
        fp=fopen(filepath,"r");
        if(fscanf(fp,"%d%d",&nodes,&links)){
            vector<int> tempmatrix(nodes+1,-1);//老师代码的common.h
            matrix.resize(nodes+1,tempmatrix);//之前实现CAA时直接在main中用的vector重新读取，还是在初始化时直接存比较简洁
            tempmatrix.clear();
            Node tempnode;
            Link templink;
            for(int i=0;i<nodes;i++){
                fscanf(fp,"%lf",&tempnode.CPU);
                tempnode.nodeId=i+1;//物理网络的节点按文件中从上往下 1 2 3 4 5 6号来存
                tempnode.residual_cpu=tempnode.CPU;
                tempnode.ratio=0;
                mapNodes.insert(pair<int,Node>(i,tempnode));
            }
            for(int i=0;i<links;i++){
                fscanf(fp,"%d %d %lf",&templink.from,&templink.to,&templink.bandwidth);
                templink.linkId=i+1;   //边的序号从1开始  1 2 3 4 5
                templink.residual_bw=templink.bandwidth;
                templink.ratio=0;
                mapLinks.insert(pair<int,Link>(i,templink));
                matrix[templink.from][templink.to]=templink.bandwidth;
                matrix[templink.to][templink.from]=templink.bandwidth;
            }
            fclose(fp);
        }
	}
	Node getnodebyId(int id){
        int i;
        for( i=0;i<nodes;i++){
            if(mapNodes.at(i).nodeId==id){
                return mapNodes.at(i);
            }
        }
    }
    void print(){//用来看节点排序情况和
//        for(int i=0;i<nodes;i++){
//                cout<<mapNodes.at(i).nodeId<<"  ";
//        }
        for(int i=1;i<nodes+1;i++){
            for(int j=1;j<nodes+1;j++){
                cout<<matrix[i][j]<<' ';
            }
            cout<<endl;
        }
	}
	double getNodeLinkedBW(int nodeid){
        double sumbw=0;
        for(int i=0;i<mapLinks.size();i++){
            if(mapLinks[i].from==nodeid){
                sumbw+=mapLinks[i].bandwidth;
            }
        }
        for(int i=0;i<mapLinks.size();i++){
            if(mapLinks[i].to==nodeid){
                sumbw+=mapLinks[i].bandwidth;
            }
        }
        return sumbw;
	}
	void sortNodes(){
        double cpu_bw[mapNodes.size()];
        for(int i=0;i<mapNodes.size();i++){
            cpu_bw[i] = mapNodes[i].CPU*getNodeLinkedBW(mapNodes[i].nodeId);
        }
        for(int i=0;i<mapNodes.size()-1;i++){
            for(int j=0;j<mapNodes.size()-1-i;j++){
                if(cpu_bw[j]<cpu_bw[j+1]){
                    double tmpcpu_bw=cpu_bw[j];
                    cpu_bw[j]=cpu_bw[j+1];
                    cpu_bw[j+1]=tmpcpu_bw;
                    Node node = mapNodes.at(j);
                    mapNodes.at(j)=mapNodes.at(j+1);
                    mapNodes.at(j+1)=node;
                }
            }
        }
	}
	int first_vertex(int v){
        int i;
        if (v<0 || v>nodes)
            return -1;
        for (i = 0; i <=nodes; i++)
            if (matrix[v][i]!=0 && matrix[v][i]!=-1)
                return i;
        return -1;
    }
    int next_vertix(int v,int w){
        int i;
        if (v<0 || v>nodes || w<0 || w>nodes)
            return -1;
        for (i = w+1 ; i <= nodes; i++)
            if (matrix[v][i]!=0 && matrix[v][i]!=-1)
                return i;
        return -1;
    }
};
//struct SNUnit{//物理网络临时拷贝备份：目的是为了进行删掉不符合条件的边，进行最短路径查找。
//public:
//	int nodes;//节点的个数
//	int links;//链路的条数
//	//vector<vector<int> > matrix;
//	//为了使用Floyd最短路径算法而设置的变量
//	vector<vector<int> > path;//路径 指明前驱
//	vector<vector<double> > dist;//距离
//};

void BFS_VNM(SubstrateNetwork &substrateNetwork,Request &request,map<int,int> &resultNodes,map<int,int>&resultLinks){
	bool sNodeMapped[substrateNetwork.nodes];
	bool reqNodeMapped[request.nodes];
	memset(sNodeMapped,false,substrateNetwork.nodes);
	memset(reqNodeMapped,false,request.nodes);
    int reqQue[request.nodes];
    int sQue[substrateNetwork.nodes];
    int reqhead = 0;
    int reqrear = 0;
    int sqhead  = 0;
    int sqrear  = 0;
    int w,q;
	for (int i = 0; i < request.nodes; ++i){
        Node p;
		if(!reqNodeMapped[i]){
            p = substrateNetwork.mapNodes.at(i);

            if(p.CPU > request.mapNodes.at(i).CPU){
                resultNodes.insert(pair<int,int>(request.mapNodes.at(i).nodeId,p.nodeId));
//                mappinglinks
//                resultLinks.insert(pair<int,int>(w,resultNodes.at(i).get(w)));
                reqNodeMapped[request.mapNodes.at(i).nodeId]=true;
                sNodeMapped[p.nodeId]=true;
                reqQue[reqrear++]=request.mapNodes.at(i).nodeId;
                sQue[sqrear++] = p.nodeId;
            }
            while(reqhead!=reqrear){
                int u = reqQue[reqhead++];//u是nodeid  不是序号,因为此时节点顺序已经排序完毕
                int p = sQue[sqhead++];
                q = substrateNetwork.first_vertex(p);//返回第一个邻接点的序号,也是nodeid；
                for(w=request.first_vertex(u);w>=0;w=request.next_vertix(u,w)){
                    if(!reqNodeMapped[w]&&!sNodeMapped[q]){
                        if((substrateNetwork.getnodebyId(q).CPU>request.getnodebyId(w).CPU)
//                        &&(substrateNetwork.getbandwidth(p,q)>request.getbandwidth(u,w))
                        ){
                            resultNodes.insert(pair<int,int>(w,q));
                            //mappinglink
//                            resultLinks.insert(pair<int,int>(w,resultNodes.get(w)));
                            reqQue[reqrear++]= w;
                            sQue[sqrear++]   = q;
                            reqNodeMapped[w]=true;
                            sNodeMapped[q]=true;
//                            cout<<"while \n";
                        }
                    }
                    //updatebandwidth(p,q);
                    q=substrateNetwork.next_vertix(p,q);
                    if(q == -1){
                        break;
                    }
//                    cout<<"id = "<<w<<" for\n"<<endl;
                }
            }
        }
	}
}
int main()
{
    SubstrateNetwork substrateNetwork;Request request;
    char filepath1[]="/home/waynamigo/Desktop/myDocuments/c++/substratenet";
    char filepath2[]="/home/waynamigo/Desktop/myDocuments/c++/requests";
    substrateNetwork.initSubstrateNetwork(filepath1);
    substrateNetwork.sortNodes(); //按论文中的（CPU×邻接链路带宽和）从大到小对物理节点排序
    request.initRequest(filepath2);
    request.sortNodes();
    cout<<"节点根据本文的节点资源度量模型（CPU×邻接链路带宽和）排序后：\n";
    cout<<"物理节点顺序\n";substrateNetwork.print();cout<<endl;
    cout<<"虚拟节点顺序\n";request.print();cout<<endl;
    map<int,int> resNodes;
    map<int,int> resLinks;
    BFS_VNM(substrateNetwork,request,resNodes,resLinks);

    cout<<"节点映射的对应关系"<<endl;
    for(map<int,int>::iterator iter=resNodes.begin();iter!=resNodes.end();iter++){
        cout<<iter->first<<'\t'<<sNet[iter->second]<<endl;
    }


    cout<<"按照requests的编号,对应的物理链路映射的顺序"<<endl;
    for(map<int,int>::iterator iter=resLinks.begin();iter!=resLinks.end();iter++){
        cout<<iter->first<<'\t'<<iter->second<<endl;
    }
//    cout<<"最后 "<<2<<"条 按照k-shortestpath："<<endl;

//    cout<<"经过节点"<<endl;


    return 0;
}







