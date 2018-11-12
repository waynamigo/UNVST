#ifndef SUBSTRATE_H_INCLUDED
#define SUBSTRATE_H_INCLUDED
#include "Base.h"
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
	void initSubstrateNetwork(char *filepath);//初始化
	void Init_shortestpath(double **shortestpath,vector<int>**sparray);//借助一个最短路径的数组，记录i,j下标，进行hops的计算
	double getNodeLinkedBW(int nodeid);//用来排序的计算节点bandwidth总和
	void sortNodes();//最初的排序操作
	void testFloyd();//改成全部输出 便于手算结果
//	double getlengtg(int from,int to){
//        double length =0;
//
//	}
	void Floyd(int from,int to);//flod计算最短路径 // from,to为当前两节点对应的物理节点编号
	void print();//查看结构体内容
    Node getnodebyId(int id);//按节点编号查找节点
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
Node SubstrateNetwork::getnodebyId(int id){
    int i;
    for( i=0;i<nodes;i++){
        if(mapNodes.at(i).nodeId==id){
            return mapNodes.at(i);
        }
    }
}
void SubstrateNetwork::print(){//查看读取及排序后的节点
    for(int i=0;i<nodes;i++){
            cout<<mapNodes.at(i).nodeId<<"  ";
    }
    cout<<endl;
}
void SubstrateNetwork::Floyd(int from,int to){//flod计算最短路径{
    int row = 0;
    int col = 0;
    int dis[nodes+1][nodes+1],path[nodes+1][nodes+1];
    for (row = 0; row < nodes; row++) {
        for (col = 0; col < nodes; col++) {
            dis[row][col] = matrix[row][col];
            path[row][col] = col;
        }
    }
    //计算每个点对的最短路径
    int temp = 0;
    int select = 0;
    for (temp = 0; temp < nodes; temp++) {
        for (row = 1; row < nodes; row++) {
            for (col = 1; col < nodes; col++) {
                //为了防止溢出，所以需要引入一个select值
                select = (dis[row][temp] == -1 || dis[temp][col] == -1) ? -1 : (dis[row][temp] + dis[temp][col]);
                if (dis[row][col] > select) {
                    dis[row][col] = select;
                    path[row][col] = path[row][temp];
                }
            }
        }
    }
    cout << "输出最短路径（用编号输出），验证计算结果：" << endl;

    row = 0;col = 0;temp = 0;
    for (row = 1; row < nodes; row++) {
        for (col = row + 1; col <nodes; col++) {
            if((row == from)&&(col==to)){
                cout <<row<< " to  "  <<col << " path: " <<row;
                // " weight: "<< dis[row][col] <<
                temp = path[row][col];
                while (temp != col) {
                    cout << "-->" << temp;
                    temp = path[temp][col];
                }
                cout << "-->"<< col<< endl;
            }
        }
    }
}
void SubstrateNetwork::testFloyd(){//改成全部输出 便与手算结果核对
    int row = 0;
    int col = 0;
    int dis[nodes+1][nodes+1],path[nodes+1][nodes+1];
    for (row = 0; row < nodes; row++) {
        for (col = 0; col < nodes; col++) {
            dis[row][col] = matrix[row][col];
            path[row][col] = col;
        }
    }
    //计算每个点对的最短路径
    int temp = 0;
    int select = 0;
    for (temp = 0; temp < nodes; temp++) {
        for (row = 0; row < nodes; row++) {
            for (col = 0; col < nodes; col++) {
                //为了防止溢出，所以需要引入一个select值
                select = (dis[row][temp] == -1 || dis[temp][col] == -1) ? -1 : (dis[row][temp] + dis[temp][col]);
                if (dis[row][col] > select) {
                    dis[row][col] = select;
                    path[row][col] = path[row][temp];
                }
            }
        }
    }
    cout << "输出最短路径（用编号输出），验证计算结果：" << endl;
    row = 0; col = 0;temp = 0;
    for (row = 0; row < nodes; row++) {
        for (col = row + 1; col <nodes; col++) {
            cout <<row + 1<< " to   "  <<col+1 << " weight: "<< dis[row][col] << " path: " <<row + 1;
            temp = path[row][col];
            while (temp != col) {
                cout << "-->" << temp + 1;
                temp = path[temp][col];
            }
            cout << "-->"<< col + 1 << endl;
        }
        cout << endl;
    }
}
void SubstrateNetwork::initSubstrateNetwork(char *filepath){//初始化
    FILE *fp;
    fp=fopen(filepath,"r");
    if(fscanf(fp,"%d %d",&nodes,&links)){
        vector<int> tempmatrix(nodes,-1);//from common.h
        matrix.resize(nodes,tempmatrix);//之前实现CAA时直接在main中用的vector重新读取，还是在初始化时直接存比较简洁
        totalBW=0;
        totalCPU=0;
        tempmatrix.clear();
        Node tempnode;
        Link templink;
        for(int i=0;i<nodes;i++){
            fscanf(fp,"%d %d %lf",&tempnode.x,&tempnode.y,&tempnode.CPU);
            tempnode.nodeId=i;//=i+1;更改，物理网络的节点按文件中从上往下0号来存
            tempnode.residual_cpu=tempnode.CPU;
            tempnode.ratio=0;
            totalCPU+=tempnode.CPU;
            mapNodes.insert(pair<int,Node>(i,tempnode));
        }
        for(int i=0;i<links;i++){
            double nouse;
            fscanf(fp,"%d %d %lf %lf",&templink.from,&templink.to,&templink.bandwidth,&nouse);// the forth num is not used
            templink.linkId=i;//边的序号从1开始  1 2 3 4 5
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
void SubstrateNetwork::Init_shortestpath(double **shortestpath,vector<int>**sparray){//借助一个最短路径的数组，记录i,j下标，进行hops的计算
    for(int i=0;i<nodes;i++){
        shortestpath[i]=new double[nodes];
        sparray[i]=new vector<int>[nodes];//初始化数组对象
    }
    for(int i=0;i<nodes;i++){
        for(int j=0;j<nodes;j++){
            //※※※※※※※※※※※※※※※※※※※※※※※※※※
            vector<int> tempvec;
            tempvec.clear();
            sparray[i][j]=tempvec;
            //※※※※※※※※※※※※※※※※※※※※※※※※※※
            if(i==j){
                shortestpath[i][j]=0;//为了避免除数为零
            }else{
                shortestpath[i][j]=6.0;//这里只考虑最大跳数为5的情况
            }
        }
    }
}
double SubstrateNetwork::getNodeLinkedBW(int nodeid){//计算节点bandwidth总和，用在最初的排序
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
void SubstrateNetwork::sortNodes(){//最初的排序操作
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
#endif // SUBSTRATE_H_INCLUDED
