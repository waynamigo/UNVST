#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED
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
#include<string>
using namespace std;
#define MAX_REQ_PER_NODE 1000
#define MAX_REQ_LINKS 100
struct Link{
	int linkId;
	int from;
	int to;
	double bandwidth;
	double residual_bw;
	double ratio;
	double weight;
	int hostVirtualLinkCount;//已经映射了虚拟链路的条数
	int requestIndex[MAX_REQ_LINKS];//服务的虚拟网络请求的索引数组
	int virtualLink[MAX_REQ_LINKS];//虚拟请求链路数组
	double BW[MAX_REQ_LINKS];//带宽数组
};
struct Node{
	int nodeId;
	double CPU;
	double residual_cpu;
	double ratio;
	int hops;
	int hostVirtualNodeCount;//该节点已经映射的虚拟节点个数
	int requestIndex[MAX_REQ_PER_NODE];//虚拟请求索引数组，用以存放驻留在该节点的虚拟请求的Index
	int virtualNode[MAX_REQ_PER_NODE];//虚拟节点数组，
	double CPUArray[MAX_REQ_PER_NODE];//CPU数组，用以存放驻留在该节点的虚拟请求的CPU
	int transferVirtualLinkCount;//中转的虚拟链路条数
	set<int> transferLinkSet;//中转的链路索引集合
	set<int> linkNeighbors;//存放邻居链路的索引号
	set<int> nodeNeighbors;//存放邻居节点的索引号
    int x,y;
};
struct shortestpath {
	int length;
	int next;
	double bw;
};
struct SNUnit{
	int nodes;
	int links;
	vector<vector<int> > path;
	vector<vector<double> > dist;
};
#endif // BASE_H_INCLUDED
