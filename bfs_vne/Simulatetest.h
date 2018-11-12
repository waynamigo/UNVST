#include<queue>
#include<vector>
#include"Request.h"

#define EVENT_ARRIVE  0
#define EVENT_DEPART  1
#define EVENT_LOG   3
#define EVENT_END_SIM 2
#define EVENT_SECOND 4
#define TIME_INTERVAL 100

class Event{
public:
	int type;
	int time;
	int index;
	Event(int type_,int time_,int index_){
		type=type_;
		time=time_;
		index=index_;
	}
};
class EventComparer{//用操作符（）来比较两个元素的排序方式，（e1,e2）
    public:
        bool operator()(const Event& e1, const Event& e2) const {
            if(e1.time > e2.time)
                return 1;
            else
                return 0;
        }
};

class Simulatetest
{
public:
	priority_queue<Event, vector<Event>, EventComparer> PQ;
	Simulatetest(vector<Request> &req){
		cout<<"enter simulator constructor "<<endl;
		for(unsigned int i=0; i<req.size(); i++){
			PQ.push(Event(EVENT_ARRIVE, req[i].arrival, i));
		}
		for(unsigned int i = 1; i <= 12; i++){
			PQ.push(Event(EVENT_LOG,i*800,0));
		}
	}

};
