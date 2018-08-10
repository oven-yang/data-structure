#include<iostream>
#include<string>
#include"../trie_tree/trie_tree.h"

using namespace std;

int main()
{
    trie_tree<> t;
    string s1{"abc"}, s2{"abaaa"}, s3{"abaab"};
    t.insert(s1.begin(), s1.end(), "abc");
    cout<<s1<<endl;
    t.insert(s2.begin(), s2.end(), "abaaa");
    cout<<s2<<endl;
    t.insert(s3.begin(), s3.end(), "abaab");
    cout<<s3<<endl;
    string s{"abaadef"};
    auto p = t.find(s.begin(), s.end());
    cout<<p->info<<endl;
}