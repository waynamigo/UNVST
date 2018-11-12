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
struct SNUnit{
	int nodes;
	int links;
	vector<vector<int> > path;
	vector<vector<double> > dist;
};
#endif // BASE_H_INCLUDED
