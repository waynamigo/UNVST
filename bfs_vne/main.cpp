#include <iostream>
#include"Base.h"
#include"Request.h"
#include"Substrate.h"
#include"Simulatetest.h"
#include<string>
using namespace std;
void test(Request request){
    int from,to;
    from =1;
    to =2;
    cout<<request.iflinked(from,to);
}
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
	for(int k=0;k<snunit.nodes;k++){//最外层循环一定是K，使用的是Floyd算法求解最短路径
		for(int m=0; m<snunit.nodes ; m++){
			for(int n=0; n<snunit.nodes; n++){//!(fabs(snunit.dist[m][k]-999999)<0.001||fabs(snunit.dist[k][n]-999999)<0.001)&&
				if(snunit.dist[m][n] > snunit.dist[m][k] + snunit.dist[k][n]){
					snunit.dist[m][n] = snunit.dist[m][k] + snunit.dist[k][n];
					snunit.path[m][n] = snunit.path[m][k];
				}
			}
		}
	}
	return true;
}
double getCost(Request &request,map<int,int> &resultNodes,map<int,int> &resultLinks){
    double cost=0;
	double cpu_revenue = 0;
	double bw_revenue = 0;
	for(int i=0;i<request.nodes;i++){
		cpu_revenue += request.mapNodes[i].CPU;//计算一组（一个request）请求的总CPU
	}
	for(int i=0; i<request.links; i++){
		bw_revenue +=(request.mapLinks[i].bandwidth*(resultLinks.size()-1));//计算bandwidth
	}
	cost=(cpu_revenue + bw_revenue)*(request.duration);
	return cost;
}
double getRev(Request &request,map<int,int> &resultNodes,map<int,int> &resultLinks){//vector<int>存取比较方便，但是改回来太麻烦 不改了
	double revenue=0;
	double cpu_revenue = 0;
	double bw_revenue = 0;
	for(int i=0;i<request.nodes;i++){
		cpu_revenue += request.mapNodes[i].CPU;
	}
	for(int i=0; i<request.links; i++){
		bw_revenue +=(request.mapLinks[i].bandwidth);
		if(resultLinks.size()==0){
            break;//映射失败 直接跳出，此时revenue=
        }
    }
	revenue = (cpu_revenue + bw_revenue)*(request.duration);
	return revenue;
}
void BFS_VNM(SubstrateNetwork &substrateNetwork,Request &request,map<int,int> &resultNodes,map<int,int>&resultLinks){
	int sNodeMapped[substrateNetwork.nodes]={0};
	int reqNodeMapped[request.nodes]={0};
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
		if(reqNodeMapped[request.mapNodes.at(i).nodeId]==0){
            p = substrateNetwork.mapNodes.at(i);
            if(p.CPU > request.mapNodes.at(i).CPU&&(reqNodeMapped[p.nodeId])){
                resultNodes.insert(pair<int,int>(request.mapNodes.at(i).nodeId,p.nodeId));
//                mappinglinks
                reqNodeMapped[request.mapNodes.at(i).nodeId]=1;
                sNodeMapped[p.nodeId]=1;
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
                            cout<<"req "<<w<<"sq "<<q<<endl;
                            reqNodeMapped[w]=1;
                            sNodeMapped[q]=1;
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
//     对余下的链路进行映射：
    // substrateNetwork.testFloyd();
    // for(int i=0;i<request.links;i++){//映射余下的request
    //    map<int,int> temp;
    //     if(!resultLinks.find(temp.insert(pair<int,int>(request.mapLinks.at(i).from,request.mapLinks.at(i).to))){//
    //        substrateNetwork.Floyd(request.mapLinks.at(i).from,request.mapLinks.at(i).to);
    //     }
    // }
}
double getSF(Request &request,map<int,int> &resultNodes,map<int,int>&resultLinks,SubstrateNetwork &substrateNetwork){
//虚拟链路扩张因子的计算
//虚拟链路扩张因子表示虚拟网络中 所有虚拟链路的映射路径长度 的平均值
    double sf =0;
    int vlink =0;
    for(map<int,int>::iterator iter=resultLinks.begin();iter!=resultLinks.end();iter++){
//        cout<<iter->first<<'\t'<<iter->second<<endl;
       // sf += substrateNetwork.getlength(iter->first,iter->second);
        //
    }
}
void display_bytime(Request &request,map<int,int> &resultNodes,map<int,int>&resultLinks){
    cout<<request.arrival<<"  to  "<<request.duration<<endl;
    double acc=0;
    int sum_suc=0;
    for(int i=0;i<request.links;i++){
    	if(resultLinks.at(i)==request.mapLinks.at(i).linkId){
    	    sum_suc++;
    	}
    }
    acc = (double)sum_suc/request.links;
    cout<<"accuracy is: "<<acc<<endl;
}
void testSimulator(){
    vector<Request>requests;Request request;
    char filepath2[]="/home/waynamigo/Desktop/myDocuments/c++/bfs_vne/reqs0/req";
    char filename[]="/req";//!char filepath2[]="/home/waynamigo/Desktop/myDocuments/c++/bfs_vne/reqs";

    char filepath3[100];
    for(int folder = 0;folder<5 ;folder++){
        for(int filenum =0;filenum<200;filenum++){
            sprintf(filepath3,"%s%d%s%d.txt",filepath2,folder,filename,filenum);
            request.initRequest(filepath3);
            request.sortNodes();
            requests.push_back(request);
            request.print();
            cout<<filepath3<<endl;
        }
    }
    for(int filenum = 0;filenum<10;filenum++){

            sprintf(filepath3,"%s%d.txt",filepath2,filenum);
            cout<<filepath3<<endl;
            request.initRequest(filepath3);
            request.sortNodes();
            //requests.push_back(request);
            //request.clean();
            request.print();
    }
    Simulatetest simulator(requests);   
}
int main()
{
    SubstrateNetwork substrateNetwork;Request request;SNUnit snunit;
    char filepath1[]="/home/waynamigo/Desktop/myDocuments/c++/substratenet";

    substrateNetwork.initSubstrateNetwork(filepath1);
    substrateNetwork.sortNodes();
    substrateNetwork.print();
    cout<<'\n'; //按论文中的（CPU×邻接链路带宽和）从大到小对物理节点排序
//    for(int cnt=0;cnt<200;cnt++){//进行循环输入request文件的路径，
//        char filepath2[]="/home/waynamigo/Desktop/myDocuments/c++/bfs_vne/reqs4/req";
//        char filepath3[30];
//        sprintf(filepath3,"%s%d.txt",filepath2,cnt);
//        request.initRequest(filepath3);
//        request.sortNodes();

       // test(request);
//        cout<<"将物理网络加载到第三方集合中:   ";
//        if(Initialize_SNUnit(substrateNetwork,snunit)){
//            cout<<"加载结束,无异常情况"<<endl;
//        }
//        double** shortestpath=new double*[substrateNetwork.nodes];//判断最短路的path
//        set<int>setMapped;
//        vector<int>** sparray=new vector<int>*[substrateNetwork.nodes];
//        substrateNetwork.Init_shortestpath(shortestpath,sparray);
//        cout<<"节点根据本文的节点资源度量模型（CPU×邻接链路带宽和）排序后：\n";
//        cout<<"物理节点顺序\n";substrateNetwork.print();
//        cout<<"虚拟节点顺序\n";request.print();cout<<endl;
//        map<int,int> resNodes;
//        map<int,int> resLinks;
        testSimulator();
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
//         char resultfile[]="/home/waynamigo/Desktop/myDocuments/c++/result";
//         ofstream fout(resultfile,ios::app);
//         fout<< r_c<<endl;//!<<','<<double(1000*resLinks.size())/(request.duration-request.arrival)
//         fout.close();
//    }
    return 0;
}
