#include <iostream>
#include"Base.h"
#include"Request.h"
#include"Substrate.h"
#include"Simulatetest.h"
#include<string>
#include"calculateAVE.h"
using namespace std;
void Init_shortestpath(SubstrateNetwork &substrateNetwork, double** shortestpath, vector<int>** sparray){
	//初始化二维数组,并且使用
	for(int i=0;i<substrateNetwork.nodes;i++){
		shortestpath[i]=new double[substrateNetwork.nodes];
		sparray[i]=new vector<int>[substrateNetwork.nodes];//初始化数组对象
	}
	for(int i=0;i<substrateNetwork.nodes;i++){
		for(int j=0;j<substrateNetwork.nodes;j++){
			//※※※※※※※※※※※※※※※※※※※※※※※※※※
			vector<int> tempvec;tempvec.clear();
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
bool Init_SNUnit(SubstrateNetwork &sn,SNUnit &snunit){
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
	//遍历当前物理网络中所有的边，并拷贝每一条边的节点
	for(int k=0;k<sn.links;k++){
		int snFrom=sn.mapLinks[k].from;
		int snTo=sn.mapLinks[k].to;
		// 在不考虑物理网络链路带宽不足的情况下运行
		//double snBW=sn.mapLinks[sn.GetLinkIndex(snTo,snFrom)].residual_bw;//这里用剩余带宽
		//if(snBW>=need_bw){/.
			snunit.dist[snFrom][snTo]=1;
			snunit.dist[snTo][snFrom]=1;
			snunit.path[snFrom][snTo]=snTo;//距离为1 只考虑hops
			snunit.path[snTo][snFrom]=snFrom;//距离为1 只考虑hops
		//}
	}
	for(int k=0;k<snunit.nodes;k++){
		for(int m=0; m<snunit.nodes ; m++){
			for(int n=0; n<snunit.nodes; n++){
				if(snunit.dist[m][n] > snunit.dist[m][k] + snunit.dist[k][n]){
					snunit.dist[m][n] = snunit.dist[m][k] + snunit.dist[k][n];
					snunit.path[m][n] = snunit.path[m][k];
				}
			}
		}
	}
	return true;
}
//缓存所有的最短路径
//如果路径长度超过5，就认为没有路径
bool SN_Floyd(const SNUnit &snunit,int from,int to,vector<int> &vecSP){
	int k;
	k=snunit.path[from][to];//spath[start*sn.subInfo.nodes+end].next;
	if(k==-1){//这里有可能存在没有路径的情况，一定要注意先判断
		vecSP.clear();
		return false;
	}
	vecSP.clear();//第一步，清空数据
	vecSP.push_back(from);//开始节点放到vector里面
	int count=0;//这里排除掉路径长度超过10的路径，认为不存在这样的路径。
	while(k!=to){
		vecSP.push_back(k);
		k=snunit.path[k][to];//spath[k*sn.subInfo.nodes+end].next;//逐步递推，但是存在：没有路径的情况。
		count++;
		if(count>=5){
			vecSP.clear();//数据清空，返回失败，跳出循环
			return false;
		}
	}
	vecSP.push_back(to);//最后一个节点放到vector里面
	return true;
}
//not recently used
void getRequest_node_vector(Request & request,vector<Node>vecVN);
bool node_mapping(SubstrateNetwork &sn, Request &request, map<int,int> &vnMap,double** shortestpath,vector<int>** sparray){
	//※※※※※※※※※※※※※※※※※※※※※※改进算法※※※※※※※※※※※※※※※※※※※※※※
	map<int,bool> snMapped;
	for(int i=0;i<sn.nodes;i++){//首先进行映射初始化，假定所有节点都未被映射
		snMapped[i]=false;
	}
	//这里定义一个集合，用于保存已经映射的物理节点的集合，便于计算HOPS
	set<int> setMappedSN;//已经映射的SN节点集合
	setMappedSN.clear();//#2017-08-02 张培颖 这个结果用于计算未来映射的节点和已经映射节点集合距离时使用。
	for(int i=0;i<request.mapNodes.size();i++){//对于每个VNR而言，从具有资源最大的开始进行节点映射
		double need_cpu=request.mapNodes[i].CPU;//所需求的CPU容量大小
		//2、#####物理节点的H值排序#####
		vector<int> vecSN;//存放物理网络节点的向量
		//这里需要先赋值给vecSN
		for(int k =0 ;k<sn.nodes;k++){
			vecSN.push_back(sn.mapNodes[k].nodeId);
		}
		//这里计算完之后，是按照SN节点的【H值，根据算法不同而不同】进行降序排列的
		for(int j=0;j<vecSN.size();j++){
			if(!snMapped[vecSN[j]]){//这里一定是剩余CPU greater than the required cpu capacity
				if(sn.mapNodes[vecSN[j]].residual_cpu>request.mapNodes[request.mapNodes[i].nodeId].CPU){
					vnMap[request.mapNodes[i].nodeId]=vecSN[j];//节点映射结果存放
					cout<<"result"<<vnMap[request.mapNodes[i].nodeId]<<"-->"<<vecSN[j]<<endl;
					snMapped[vecSN[j]]=true;
					setMappedSN.insert(vecSN[j]);//保存已经映射的物理节点，每个VNR保存一次
					break;//跳出循环
				}
			}
			if(j==sn.nodes){//如果没有合适的物理节点，则返回映射失败
				return false;
			}
		}
	}
	return true;
}
//not recently used
bool link_mapping(SubstrateNetwork &sn,Request &curReq,map<int,int> &node_mapping_result,map<int, vector<int>> &link_mapping_result,int algorithmType);
bool cache_shortest_path(SubstrateNetwork &sn, SNUnit &snunit, double** shortestpath,vector<int>** sparray){
	for(int i=0;i<sn.nodes;i++){
		for(int j=0;j<sn.nodes;j++){
			if(i<j){//这里为了提高效率，只考虑i<j，相反的情况把坐标交换即可。
				vector<int> vecSP;  vecSP.clear();
				if(SN_Floyd(snunit,i,j,vecSP)){
					shortestpath[i][j]=vecSP.size()*1.0;
					shortestpath[j][i]=vecSP.size()*1.0;
					sparray[i][j]=vecSP;//把最短路径缓存起来
					sparray[j][i]=vecSP;
				}
			}
		}
	}
	return true;
}
bool do_node_mapping(SubstrateNetwork &sn, Request &curReq, map<int,int> &node_mapping_result){
	//遍历所有的虚拟节点的映射结果，即map<int,int>
	for(map<int,int>::iterator it=node_mapping_result.begin(); it!=node_mapping_result.end();it++){
		sn.mapNodes[it->second].residual_cpu=sn.mapNodes[it->second].residual_cpu-curReq.mapNodes[it->first].CPU;//剩余CPU变化
		sn.mapNodes[it->second].ratio=1.0-sn.mapNodes[it->second].residual_cpu/sn.mapNodes[it->second].CPU;//CPU资源利用率
		int req_count=sn.mapNodes[it->second].hostVirtualNodeCount;//当前物理节点服务的虚拟节点的个数
		//把服务的虚拟节点的信息放到数组的最后一个元素中
		sn.mapNodes[it->second].requestIndex[req_count]=curReq.requestId;//请求的编号
		sn.mapNodes[it->second].virtualNode[req_count] = it->first;//请求的节点编号
		sn.mapNodes[it->second].CPUArray[req_count] = curReq.mapNodes[it->first].CPU;//需要的CPU
		sn.mapNodes[it->second].hostVirtualNodeCount++;//当前服务的虚拟节点个数加1
	}
	return true;
}
//打印节点映射结果
void print_node_mapping(map<int,int> &node_mapping_result){
	for(map<int,int>::iterator it=node_mapping_result.begin(); it!=node_mapping_result.end();it++){
		cout<<"vn:"<<it->first<<" mapped to  sn:"<<it->second<<endl;
	}
}
int main(){
	SubstrateNetwork substrateNetwork;Request request;SNUnit snunit;
    char substrateFile[]="/home/waynamigo/Desktop/myDocuments/c++/bfs_vne/sub.txt";
    char requestFile[]="/home/waynamigo/Desktop/myDocuments/c++/bfs_vne/reqs0/req1.txt";
    substrateNetwork.initSubstrateNetwork(substrateFile);substrateNetwork.sortNodes();
    substrateNetwork.InitEachneighbors();
    cout<<"after initSubstrateNetwork:\n";substrateNetwork.print();
    request.initRequest(requestFile);request.sortNodes();
    request.initEachNodesNeighbor();
    cout<<"after initSubstrateNetwork:\n";request.print();
    double ** shortestpath = new double *[substrateNetwork.nodes];//c++ new一个二维数组
    vector<int>** sparray=new vector<int>*[substrateNetwork.nodes];//sparray
	Init_shortestpath(substrateNetwork,shortestpath,sparray);
    if(Init_SNUnit(substrateNetwork,snunit)==true){
        cout<<"记录最短路径";
    }
    // set<int>setMapped;
    cache_shortest_path (substrateNetwork,snunit,shortestpath,sparray);
    //进行映射
    double done_rev=0;//收益
	double done_cost=0;//成本
	bool nodeMapFlag=false;//指示当前节点映射是否成功
	bool linkMapFlag=false;  //指示当前链路映射是否成功
	int accept=0;
	int count=0;		
    map<int, int> virtualNodesMap;//存放节点映射结果
	map<int,vector<int> > virtualLinksMap;//存放链路映射结果
	//执行
	nodeMapFlag= node_mapping(substrateNetwork,request,virtualNodesMap,shortestpath,sparray);
	if(nodeMapFlag){
		nodeMapFlag =  do_node_mapping(substrateNetwork,request,virtualNodesMap);
	}
	cout<<"\n"<<virtualNodesMap.size()<<endl;
	if(nodeMapFlag==1){//如果分配资源没有出错，进行链路的映射
		print_node_mapping(virtualNodesMap);
	}
	
    cout<<"以下是更新完带宽后物理网络的内容";
	substrateNetwork.print();

//         cout<<"物理节点顺序\n";substrateNetwork.print();
//         cout<<"虚拟节点顺序\n";request.print();cout<<endl;
//         map<int,int> resNodes;
    
//         map<int,int> resLinks;
//         testSimulator();
//         BFS_VNM(substrateNetwork,request,resNodes,resLinks);
//     //    cout<<"余下的未映射链路方案显示于floyd算法计算的路径\n";
//     //    for(map<int,int>::iterator iter=resNodes.begin();iter!=resNodes.end();iter++){
//     //        cout<<iter->first<<'\t'<<iter->second<<endl;
//     //    }
// //        cout<<"按照requests的编号,对应的物理链路映射的顺序"<<endl;
// //        for(map<int,int>::iterator iter=resLinks.begin();iter!=resLinks.end();iter++){
// //            cout<<iter->first<<'\t'<<iter->second<<endl;
// //        }
//         double r_c = request.getR_C(shortestpath,setMapped);
//         cout<<"收益开销比：" <<r_c<<endl;
//         char resultfile[]="/home/waynamigo/Desktop/myDocuments/c++/rc&acc&sf";
//         fprintSF(resultfile,resLinks,request);
        // ofstream fout(resultfile,ios::app);	
}