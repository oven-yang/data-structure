/* 字典树(trie tree)的 C++ 实现 by oven
 */

#ifndef TRIE_TREE_CPP_H
#define TRIE_TREE_CPP_H

#include<string>
#include<cstring>

using std::string;

template<typename Key>
struct subtract
{
    int operator () (const Key& key1, const Key& key2)
    {
        return key1 - key2;
    }
};

template<typename Data, int key_count>
struct Node
{
    typedef Data value_type;
    Node** child;
    Data info;

    Node() : child(new Node*[key_count]()) {}
    ~Node()
    {
        for(size_t i = 0; i < key_count; ++i)
        {
            if(child[i] != nullptr)
            {
                delete child[i];
            }
        }
        delete[] child;
    }
};

template<typename Data = string, typename Key = char, Key start_key = 'a',
    int key_count = 26, int step = 1, typename KeySubtract = subtract<Key>>
class trie_tree
{
public:
    typedef Node<Data, key_count> node_type;
    typedef Key key_type;
    typedef typename node_type::value_type value_type;
    typedef value_type* value_pointer;

    trie_tree() : root(new node_type) {}

    // return the first that not match.
    template<typename InputIt>
    node_type* find(InputIt first, InputIt last)
    {
        // check whether type is mayched.
        // typename InputIt::value_type a = Key();

        node_type *cur = root;
        while(first != last && cur->child[key_to_index(*first)] != nullptr)
        {
            cur = cur->child[key_to_index(*first)];
            ++first;
        }
        return cur;
    }
    template<typename InputIt>
    node_type* insert(InputIt first, InputIt last, const value_type& val)
    {
        node_type *cur = root;
        while(first != last && cur->child[key_to_index(*first)] != nullptr)
        {
            cur = cur->child[key_to_index(*first)];
            ++first;
        }
        while(first != last)
        {
            cur->child[key_to_index(*first)] = new node_type();
            cur = cur->child[key_to_index(*first)];
            ++first;
        }
        cur->info = val;
        return cur;
    }
private:
    Node<value_type, key_count>* root;

    // do not validate the key, it's developer's duty.
    size_t key_to_index(const key_type& key)
    {
        return (key - start_key)/step;
    }
};

#endif
