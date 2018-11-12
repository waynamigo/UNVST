#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED
#include "Base.h"

struct Request{
    int requestId;
    int nodes;
    int links;
    int arrival;//起始时间
    int duration;//持续时间
    map<int,Node> mapNodes;
	map<int,Link> mapLinks;
	vector<vector<int>> matrix;
	void initRequest(char *filepath);//初始化Reques
	int iflinked(int from,int to); //计算虚拟链路扩张因子的vlink参数
	Node getnodebyId(int id);
    double getReq();//计算收益
    double getCost(double **shortestpaths,set<int>&setMappedSN);//计算开销
    int hops(double **shortestpaths,set<int>&setMappedSN,int start);//计算跃点数hops
	double getNodeLinkedBW(int nodeid);
	void sortNodes();//排序函数
	void clean();
	void print();//用来看节点排序情况
	double getR_C(double **shortestpath,set<int>&setMapped){//计算收益开销比
        return 1.0*(getCost(shortestpath,setMapped)/getReq());
	}
	int first_vertex(int v){
        int i;
        if (v<0 || v>nodes)
            return -1;
        for (i = 0; i <nodes; i++)
            if (matrix[v][i]!=-1)
                return i;
        return -1;
    }
    int next_vertix(int v,int w){
        int i;
        if (v<0 || v>nodes || w<0 || w>nodes)
            return -1;
        for (i = w+1 ; i < nodes; i++)
            if (matrix[v][i]!=-1)
                return i;
        return -1;
    }
};
void Request::print(){//用来看节点排序情况
    for(int i=0;i<nodes;i++){
            cout<<mapNodes.at(i).nodeId<<"  ";
    }
    cout<<endl;
    for(int i=0;i<nodes;i++){
        for(int j=0;j<nodes;j++){
            cout<<matrix[i][j]<<' ';
        }
        cout<<endl;
    }
}
double Request::getNodeLinkedBW(int nodeid){
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
int Request::hops(double **shortestpaths,set<int>&setMappedSN,int start){
    if(setMappedSN.empty()){
        return 1;//没有，则其为相邻节点，跳数为1
    }
    int minlenth=5;
    //遍历里面所有节点，找到两者距离最小的节点作为度量标准
    for(set<int>::iterator it=setMappedSN.begin();it!=setMappedSN.end();it++){
        if(shortestpaths[*it][start] < minlenth){
            minlenth=shortestpaths[*it][start];
        }
    }
    return minlenth;
}
double Request::getCost(double **shortestpaths,set<int>&setMappedSN){//计算开销
    double cpu=0,hops_bw=0;
    for(int i=0;i<nodes;i++){
        cpu+=mapNodes.at(i).CPU;
    }
    set<int>::iterator iter=setMappedSN.begin();

    while(iter!=setMappedSN.end()){
        hops_bw+=hops(shortestpaths,setMappedSN,(*iter));
        ++iter;
    }
    return cpu+hops_bw;
}
void Request::sortNodes(){
    double cpu_bw[mapNodes.size()];
    for(int i=0;i<mapNodes.size();i++){
        cpu_bw[i] = mapNodes[i].CPU*getNodeLinkedBW(mapNodes[i].nodeId);
//            cout<<mapNodes[i].CPU<<'*'<<getNodeLinkedBW(mapNodes[i].nodeId)<<' ';
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
void Request::initRequest(char *filepath){//初始化
    FILE *fp;
    fp=fopen(filepath,"r");
    int n1,n2,n3;
    if(fscanf(fp,"%d%d%d%d%d%d%d",&nodes,&links,&n1,&arrival,&duration,&n2,&n3)){
        vector<int> tempmatrix(nodes,-1);//老师代码的common.h
        matrix.resize(nodes,tempmatrix);//之前实现CAA时直接在main中用的vector重新读取，还是在初始化时直接存比较简洁
        tempmatrix.clear();
        Node tempnode;
        Link templink;
        for(int i=0;i<nodes;i++){
            fscanf(fp,"%d %d %lf",&tempnode.x,&tempnode.y,&tempnode.CPU);
            tempnode.nodeId=i;//物理网络的节点按文件中从上往下 1 2 3 4 5 6号来存
            tempnode.residual_cpu=tempnode.CPU;
            tempnode.ratio=0;
            mapNodes.insert(pair<int,Node>(i,tempnode));
        }
        double nouse;
        for(int i=0;i<links;i++){
            fscanf(fp,"%d %d %lf %lf",&templink.from,&templink.to,&templink.bandwidth,&nouse);
            templink.linkId=i;   //边的序号从1开始  1 2 3 4 5
            templink.residual_bw=templink.bandwidth;
            templink.ratio=0;
            mapLinks.insert(pair<int,Link>(i,templink));
            matrix[templink.from][templink.to]=templink.bandwidth;
            matrix[templink.to][templink.from]=templink.bandwidth;
        }
        fclose(fp);
    }
}
double Request::getReq(){
    double cpu=0,bw=0;
    for(int i=0;i<nodes;i++){
        cpu+=mapNodes.at(i).CPU;
    }
    for(int i=0;i<links;i++){
        bw+=mapLinks.at(i).bandwidth;
    }
    return cpu+bw;
}
Node Request::getnodebyId(int id){
    int i;
    for( i=0;i<nodes;i++){
        if(mapNodes.at(i).nodeId==id){
            return mapNodes.at(i);
        }
    }
}
void Request::clean(){
    map<int,Node>::iterator it;
    for(it=mapNodes.begin();it!=mapNodes.end();it++){
        mapNodes.erase(it);
    }
}
int Request::iflinked(int from,int to){ //计算虚拟链路扩张因子vlink参数
    if(from<nodes&&to<nodes){
        return matrix[from][to]==-1? 0:1;
    }
    else{
        cout<<"error:虚拟链路节点编号大于节点的最大编号\n";
        return -1;
    }
}
#endif // REQUEST_H_INCLUDED
