#ifndef _OS_H_
#define _OS_H_

#include <list>
#include <algorithm>
#include <iostream>
#include <curses.h>
#include "memNode.h"
using namespace std;

enum flags{
    SIZE_ASC,
    SIZE_DESC,
    ADDR_ASC,
    NO_LOOP,
    LOOP
};
/*
const int SIZE_DESC = 0;
const int SIZE_ASC = 1;
const int ADDR_ASC = 2;
const int NO_LOOP = 3;
const int LOOP = 4;
*/
int protosize[6];
struct simulator_OS
{
    simulator_OS():_initNum(6),_minSize(0), _empty(), _use() {}
    void create_empty(char *filepath){
        FILE *fp;
        fp=fopen(filepath,"r");
        for(int i = 0; i < _initNum; ++i){
            memNode node;
            int begin, end;
            fscanf(fp,"%d %d",&begin,&end);
            node.set_node(begin, end);
            _empty.push_back(node);
            protosize[i]=node.get_size();
        }
        fclose(fp);
    }

    void list_sort(){
        _empty.sort(addr);
    }
    bool alloc(int size, list<memNode>::iterator & start){//, int flag
        auto it = start;
        for(; it != _empty.end(); ++it){
            if((it)->get_size() - size > _minSize){
                memNode node;
                node.set_node((it)->get_begin(), (it)->get_begin() + size);
                (it)->cut_node(size);
                _use.push_back(node);
                start = ++it;
                return true;
            }
            else if((it)->get_size() - size >= 0){
                _use.push_back(*it);
                _empty.erase(it);
                start = ++it;
                return true;
            }
        }
        if(it == _empty.end()){
            for(it = _empty.begin(); it != start; ++it){
                if((it)->get_size() - size > _minSize){
                    memNode node;
                    node.set_node((it)->get_begin(), (it)->get_begin() + size);
                    (it)->cut_node(size);
                    _use.push_back(node);
                    start = ++it;
                    return true;
                }
                else if((it)->get_size() - size >= 0){
                    _use.push_back(*it);
                    _empty.erase(it);
                    start = ++it;
                    return true;
                }
            }
            if(it == start)
                return false;
        }

        return true;
    }
    void output_list(const list<memNode>& list){
        int cnt=0,cnt1=1;
        char *a[] ={"#","0","5","9","17","28","36"};
        for(auto e:list){
            cnt++;
            cnt1+=2;
            int length = e.get_size();
            char s[length];
            char line[protosize[cnt-1]];
            memset(s,'|', sizeof(s));
            memset(line,'-', sizeof(line));
//            printf("┌──────┐");printf("└──────┘");

            s[length]='\0';
            line[protosize[cnt-1]]='\0';
            mvaddstr(cnt1,8,a[cnt]);
            mvaddstr(cnt1,10,s);
            mvaddstr(cnt1+1,10,line);
        }
    }
    void output(){
        cout << endl;
        output_list(_empty);
//        output_list(_use);
        cout << endl;
    }
    void distribution(){
        int size;
        mvaddstr(17,10,"input the size of process:");
        scanw("%d",&size);
        auto sit = _empty.begin();
        auto it = _empty.begin();
        bool ret;
		ret = alloc(size, it);//, flag
        if(ret == false){
            cout << "segmentation fault!!" << endl;
            return;
        }
    }

    void freed(){
        int addr;
        mvaddstr(17,10,"free from addr=");
        scanw("%d",&addr);
        auto it = _use.begin();
        for( ;it != _use.end(); ++it){
            if((it)->get_begin() == addr){
                _empty.push_back(*it);
                _use.erase(it);
                return;
            }
        }
        if(it == _use.end()){
            output();

            cout << "addr err!" << endl;
        }
    }

    void merge(list<memNode> &l){
        auto it = l.begin();
        for( ;it != l.end(); ++it){
            auto tmp = it;
            ++it;
            if(it == l.end())break;
            if(tmp->get_end() == (it)->get_begin()){
                (tmp)->set_node((tmp)->get_begin(), (it)->get_end());
                l.erase(it);
            }
            it = tmp;
        }
    }
    void FF(){
        initscr();
        cbreak();
        nonl();
        echo();
        refresh();

        merge(_empty);
        list_sort();
        output();
        int c;
        while(1){
            refresh();

            mvaddstr(16,10,"distribute=1---------release=2 :");
            scanw("%d",&c);
            if(c==1){
                distribution();
            } else if(c==2){
                freed();
            } else{
                break;
            }
            list_sort();
            merge(_empty);
            clear();
            output();

        }
    }
    int _initNum;
    int _minSize;
    list<memNode> _empty;
    list<memNode> _use;
    struct less_than{
        bool operator()(const memNode n1,const memNode n2){
            if(n1.get_size() > n2.get_size())return true;
            return false;
        }
    }less_than;
    struct greater{
        bool operator()(const memNode n1,const memNode n2){
            if(n1.get_size() > n2.get_size())return true;
            return false;
        }
    }greater;
    struct addr{
        bool operator()(const memNode n1, const memNode n2){
            if(n1.get_begin() < n2.get_begin())return true;
            return false;
        }
    }addr;
};

#endif
