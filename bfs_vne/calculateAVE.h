#include <iostream>
#include <fstream>
#include "Request.h"
#include "Base.h"
using namespace std;
void fprintSF(char *resfile,map<int,int> &reslinks,Request &request){
	char res1[]="/home/waynamigo/Desktop/myDocuments/c++/bfs_vne/rc&acc&sf";
	ofstream fout(resfile,ios::app);
	double r_c;
	for(int i=0;i<14;i++){
		if(r_c!=0){
			fout<<r_c<<','<<double(1000*reslinks.size())/(request.duration-request.arrival);
			
		}
	}
	fout.close();
	cout<<"get result ok\n";
}
