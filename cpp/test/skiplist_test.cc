#include<iostream>
#include<string>
#include"../skiplist/skiplist.h"

using namespace std;

int main()
{
    skiplist<int, int> sl(10);
    for(int i = 0; i < 10; ++i)
        sl.insert(i, i+1000);
    for(decltype(sl)::iterator it = sl.begin(); it != sl.last(); ++it)
    {
        cout<<"("<<it->key<<", "<<it->value<<"), level: "<<it->level
            <<", forward:";
        for(int i = 0; i < it->level; ++i)
        {
            cout<<' '<<(it->forward[i] ? it->forward[i]->key : -1);
        }
        cout<<endl;
    }
}
