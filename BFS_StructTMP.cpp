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
	int hops;
    int x,y;
};
struct shortestpath {
	int length;
	int next;
	double bw;
};
class SNUnit{
public:
	int nodes;
	int links;
	vector<vector<int> > path;
	vector<vector<double> > dist;
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
	void Init_shortestpath(double **shortestpath,vector<int>**sparray){//借助一个最短路径的数组，记录i,j下标，进行hops的计算
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
	void testFloyd(){//改成全部输出 便于手算结果
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
        row = 0;col = 0;temp = 0;
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
	void Floyd(int from,int to){
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
	void print(){
        for(int i=0;i<nodes;i++){
            cout<<mapNodes.at(i).nodeId<<"  ";
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
	vector<vector<int>> matrix;
	void initRequest(char *filepath){//初始化
        FILE *fp;
        fp=fopen(filepath,"r");
        if(fscanf(fp,"%d%d",&nodes,&links)){
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
	Node getnodebyId(int id){
        int i;
        for( i=0;i<nodes;i++){
            if(mapNodes.at(i).nodeId==id){
                return mapNodes.at(i);
            }
        }
    }
    double getReq(){
        double cpu=0,bw=0;
        for(int i=0;i<nodes;i++){
            cpu+=mapNodes.at(i).CPU;
        }
        for(int i=0;i<links;i++){
            bw+=mapLinks.at(i).bandwidth;
        }
        return cpu+bw;
    }
    double getCost(double **shortestpaths,set<int>&setMappedSN){
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
    int hops(double **shortestpaths,set<int>&setMappedSN,int start){
        if(setMappedSN.empty()){
            return 1;//没有，则其为相邻节点，跳数为1
        }
        int minlenth=20;
        //遍历里面所有节点，找到两者距离最小的节点作为度量标准
        for(set<int>::iterator it=setMappedSN.begin();it!=setMappedSN.end();it++){
            if(shortestpaths[*it][start] < minlenth){
                minlenth=shortestpaths[*it][start];
            }
        }
        return minlenth;
    }
    void print(){//用来看节点排序情况
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
	double getR_C(double **shortestpath,set<int>&setMapped){
        return 1.0*(getCost(shortestpath,setMapped)/getReq());
	}
	int first_vertex(int v){
        int i;
        if (v<0 || v>nodes)
            return -1;
        for (i = 0; i <nodes; i++)
            if (matrix[v][i]!=0 && matrix[v][i]!=-1)
                return i;
        return -1;
    }
    int next_vertix(int v,int w){
        int i;
        if (v<0 || v>nodes || w<0 || w>nodes)
            return -1;
        for (i = w+1 ; i < nodes; i++)
            if (matrix[v][i]!=0 && matrix[v][i]!=-1)
                return i;
        return -1;
    }
};
bool Initialize_SNUnit(SubstrateNetwork &sn,SNUnit &snunit){
	//张培颖 2016-01-31 采用另外一种方式，先去掉不满足带宽的边，然后使用Floyd最短路径计算边的映射方案
	int N=sn.nodes;
	snunit.nodes=sn.nodes;//节点个数
	snunit.links=sn.links;//边的条数
	//下面三条语句初始化matrix矩阵，值都为-1
	vector<int> innerMatrix(N,-1);//N代表节点的个数，-1代表初始化的值
	snunit.path.resize(N,innerMatrix);//path[][]的初始化也都为-1
	innerMatrix.clear();//释放内存空间
	//下面的语句初始化距离，默认都是最大距离+∞
	vector<double> innerDist(N,99999999);
	snunit.dist.resize(N,innerDist);
	innerDist.clear();
	//初始化
	for(int m=0;m<N;m++){
		for(int n=0;n<N;n++){
			if(m==n){
				snunit.dist[m][n]=0;
				snunit.path[m][n]=m;
			}else{
				snunit.dist[m][n]=99999999;
				snunit.path[m][n]=-1;//no way out
			}
		}
	}
	//遍历当前物理网络中所有的边
	for(int k=0;k<sn.links;k++){
		//取出每一条物理路径
		int snFrom=sn.mapLinks[k].from;
		int snTo=sn.mapLinks[k].to;
		//double snBW=sn.mapLinks[sn.GetLinkIndex(snTo,snFrom)].residual_bw;//这里用剩余带宽
		//if(snBW>=need_bw){
//        snunit.dist[snFrom][snTo]=1;
//        snunit.dist[snTo][snFrom]=1;
//        snunit.path[snFrom][snTo]=snTo;//距离为1 只考虑hops
//        snunit.path[snTo][snFrom]=snFrom;//距离为1 只考虑hops
		//}
	}
	//最外层循环一定是K，使用的是Floyd算法求解最短路径
	for(int k=0;k<snunit.nodes;k++){
		for(int m=0; m<snunit.nodes ; m++){
			for(int n=0; n<snunit.nodes; n++){//!(fabs(snunit.dist[m][k]-999999)<0.001||fabs(snunit.dist[k][n]-999999)<0.001)&&
				if(snunit.dist[m][n] > snunit.dist[m][k] + snunit.dist[k][n]){
					//修改两个变量的值，一个是距离，一个是指示路径
					snunit.dist[m][n] = snunit.dist[m][k] + snunit.dist[k][n];
					snunit.path[m][n] = snunit.path[m][k];
				}
			}
		}
	}
	return true;
}
void BFS_VNM(SubstrateNetwork &substrateNetwork,Request &request,map<int,int> &resultNodes,map<int,int>&resultLinks){
	bool sNodeMapped[substrateNetwork.nodes];
	bool reqNodeMapped[request.nodes];
	memset(sNodeMapped,false,substrateNetwork.nodes);
	memset(reqNodeMapped,false,request.nodes);
    int reqQue[request.nodes];
    int sQue[substrateNetwork.nodes];
    resultNodes.clear();
    resultLinks.clear();
    int reqhead = 0;
    int reqrear = 0;
    int sqhead  = 0;
    int sqrear  = 0;
    int w,q;
	for (int i = 0; i < request.nodes; ++i){
        Node p;
		if(!reqNodeMapped[i]){
            p = substrateNetwork.mapNodes.at(i);
            if(p.CPU > request.mapNodes.at(i).CPU&&(!reqNodeMapped[p.nodeId])){
                resultNodes.insert(pair<int,int>(request.mapNodes.at(i).nodeId,p.nodeId));
//                mappinglinks
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
                            reqQue[reqrear++]= w;
                            sQue[sqrear++]   = q;
                            reqNodeMapped[w]=true;
                            sNodeMapped[q]=true;
//                            cout<<"while \n";
                        }
                    }
                    q=substrateNetwork.next_vertix(p,q);
                    if(q == -1){
                        break;
                    }
//                    cout<<"id = "<<w<<" for\n"<<endl;
                }
            }
        }
	}
	for(int i=0;i<request.links;i++){   //mappinglink
        int subfrom,subto;
        for(map<int,int>::iterator iter=resultNodes.begin();iter!=resultNodes.end();iter++){
            if(request.mapLinks.at(i).from==iter->first)
                subfrom=iter->second;
        }
        for(map<int,int>::iterator iter=resultNodes.begin();iter!=resultNodes.end();iter++){
            if(request.mapLinks.at(i).to==iter->first)
                subto=iter->second;
        }
        resultLinks.insert(pair<int,int>(subfrom,subto));
    }
    // 对余下的链路进行映射：
//     substrateNetwork.testFloyd();
//     for(int i=0;i<request.links;i++){//映射余下的request
//        map<int,int> temp;
// //        if(!resultLinks.find(temp.insert(pair<int,int>(request.mapLinks.at(i).from,request.mapLinks.at(i).to)))){//
//            substrateNetwork.Floyd(request.mapLinks.at(i).from,request.mapLinks.at(i).to);
// //        }
//     }
}
int main()
{
    SubstrateNetwork substrateNetwork;Request request;SNUnit snunit;
    char filepath1[]="/home/waynamigo/Desktop/myDocuments/c++/substratenet";
    char filepath2[]="/home/waynamigo/Desktop/myDocuments/c++/requests";
    substrateNetwork.initSubstrateNetwork(filepath1);
    substrateNetwork.sortNodes(); //按论文中的（CPU×邻接链路带宽和）从大到小对物理节点排序
//    substrateNetwork.print();cout<<'\n';
    request.initRequest(filepath2);
    request.sortNodes();
//    request.print();
    cout<<"将物理网络加载到第三方集合中去:   ";
    if(Initialize_SNUnit(substrateNetwork,snunit)){
        cout<<"success"<<endl;
    }
    double** shortestpath=new double*[substrateNetwork.nodes];//判断最短路的path
    set<int>setMapped;
	vector<int>** sparray=new vector<int>*[substrateNetwork.nodes];
	substrateNetwork.Init_shortestpath(shortestpath,sparray);
    cout<<"节点根据本文的节点资源度量模型（CPU×邻接链路带宽和）排序后：\n";
    cout<<"物理节点顺序\n";substrateNetwork.print();cout<<endl;
    cout<<"虚拟节点顺序\n";request.print();cout<<endl;
    map<int,int> resNodes;
    map<int,int> resLinks;
    BFS_VNM(substrateNetwork,request,resNodes,resLinks);
    cout<<"节点映射的对应关系"<<endl;
    cout<<"余下的未映射链路方案先显示见上 ↑编号1对应A，2对应B)\n";
    for(map<int,int>::iterator iter=resNodes.begin();iter!=resNodes.end();iter++){
        cout<<iter->first<<'\t'<<iter->second<<endl;
    }
    cout<<"按照requests的编号,对应的物理链路映射的顺序"<<endl;
    for(map<int,int>::iterator iter=resLinks.begin();iter!=resLinks.end();iter++){
        cout<<iter->first<<'\t'<<iter->second<<endl;
    }
    // for (int i = 0; i < substrateNetwork.nodes; ++i)
    // {
    //     for (int j = 0; j < sizeof(shortestpath[i]); ++j)
    //     {
    //         cout<<shortestpath[i][j]<< ' ';
    //     }
    //     cout<<'\n';
    // }
    double r_c = request.getR_C(shortestpath,setMapped);
    cout<<"收益开销比：" <<r_c<<endl;

    char resultfile[]="/home/waynamigo/Desktop/myDocuments/c++/result";
    ofstream fout(resultfile,ios::app);
    fout<< r_c<<','<<recv<<endl;
    fout.close();
    return 0;
}
