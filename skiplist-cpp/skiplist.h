/* 跳表(skiplist)的 C++ 实现 by oven
 */

#ifndef SKIPLIST_CPP_H
#define SKIPLIST_CPP_H

#include<iterator>
#include<iostream>
#include<cstring>
#include<random>
#include<utility>
#include<cstdlib>
#include<cstddef>

template<typename Key, typename Value>
class skiplist_node
{
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef skiplist_node<key_type, value_type> node_type;

    skiplist_node(const key_type &key, const value_type &value, 
            size_t level = 0)
        : key(key), value(value), level(level)
    {
        forward = new node_type*[level];
        if(forward == nullptr)
        {
            std::cerr<<
                "error:skiplist_node ctor new node_type* return nullptr\n";
        }
        
    }
    ~skiplist_node()
    {
        delete []forward;
    }

    // do not check whether i is valid, 0 <= i < level
    void set_forward(int i, node_type* forward_i)
    {
        forward[i] = forward_i;
    }

    node_type* get_forward(int i)
    {
        return forward[i];
    }
    const key_type key;
    value_type value;
    int level;
    node_type** forward;
};

template<typename Node>
struct skiplist_iterator
{
public:
    typedef std::forward_iterator_tag iterator_categpry;
    typedef Node value_type;
    typedef int difference_type;
    typedef Node* pointer;
    typedef Node& reference;
    typedef const Node* const_pointer;
    typedef const Node& const_reference;
    skiplist_iterator() : node(nullptr) {}
    explicit skiplist_iterator(Node* node) : node(node) {}
    bool operator ==(const skiplist_iterator &other) const
    {
        return node == other.node;
    }
    bool operator !=(const skiplist_iterator &other) const
    {
        return !(*this == other);
    }
    skiplist_iterator& operator ++()
    {
        if(node != nullptr)
        {
            node = node->forward[0];
        }
        return *this;
    }
    skiplist_iterator& operator ++(int)
    {
        skiplist_iterator tmp(*this);
        ++*this;
        return tmp;
    }
    reference operator *()
    {
        return *node;
    }
    pointer operator ->()
    {
        return node;
    }
    const_reference operator *() const
    {
        return *node;
    }
    const_pointer operator ->() const
    {
        return node;
    }
private:
    Node* node;
};

template<typename Key, typename Value>
class skiplist
{
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef skiplist_node<key_type, value_type> node_type;
    
    typedef skiplist_iterator<node_type> iterator;
    typedef const iterator const_iterator;

    skiplist(int max_level, double probability = 0.5)
        : max_level(max_level), probability(probability)
    {
        forward = new node_type*[max_level];
        if(forward == nullptr)
        {
            std::cerr<<"error:skiplist ctor new node_type*[max_level]"
                " return nullptr\n";
            abort();
        }
        memset(forward, 0, sizeof(node_type*) * max_level);
    }
    ~skiplist()
    {
        delete []forward;
    }
    std::pair<bool, value_type> find(const key_type &key);
    void insert(const key_type &key, const value_type &value);
    void remove(const key_type &key);
    iterator begin();
    iterator last();
    const_iterator cbegin() const;
    const_iterator clast() const;
private:
    int random_level();
    int max_level;
    node_type** forward;
    double probability;
};

template<typename Key, typename Value>
int skiplist<Key, Value>::random_level()
{
    int node_level = 1;
    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_real_distribution<> dis(0, 1);
    while(node_level < max_level && dis(gen) < probability)
    {
        ++node_level;
    }
    return node_level;
}

template<typename Key, typename Value>
std::pair<bool, typename skiplist<Key, Value>::value_type>
skiplist<Key, Value>::find(const key_type &key)
{
    node_type** current = forward;
    for(int i = max_level - 1; i >= 0; --i)
    {
        while(current[i] != nullptr && current[i]->key < key)
        {
            current = current[i]->forward;
        }
    }
    return (current[0] != nullptr && key == current[0]->key) ?
        std::make_pair(true, current[0]->value) :
        std::make_pair(false, value_type());
}

template<typename Key, typename Value>
void skiplist<Key, Value>::insert(const key_type &key, const value_type &value)
{
    node_type** current = forward;
    node_type*** update = new node_type**[max_level];

    for(int i = max_level - 1; i >= 0; --i)
    {
        while(current[i] != nullptr && current[i]->key < key)
        {
            current = current[i]->forward;
        }
        update[i] = &current[i];
    }

    // if the same key exists in skiplist, replace old value with new value
    if(current[0] != nullptr && current[0]->key == key)
    {
        current[0]->value = value;
    }
    else
    {
        int new_node_level = random_level();
        node_type* new_node = new node_type(key, value, new_node_level);
        if(new_node == nullptr)
        {
            std::cerr<<"error:skiplist::insert new node_type return nullptr\n";
            abort();
        }
        while(--new_node_level >= 0)
        {
            //new_node->set_forward(new_node_level, *update[new_node_level]);
            new_node->forward[new_node_level] = *update[new_node_level];
            *update[new_node_level] = new_node;
        }
    }

    delete []update;
}

template<typename Key, typename Value>
void skiplist<Key, Value>::remove(const key_type &key)
{
    node_type** current = forward;
    node_type*** update = new node_type**[max_level];

    for(int i = max_level - 1; i >= 0; --i)
    {
        while(current[i] != nullptr && current[i]->key < key)
        {
            current = current[i]->forward;
        }
        update[i] = &current[i];
    }

    if(current[0] != nullptr && current[0]->key == key)
    {
        int delete_level = current[0]->level;
        node_type* node = current[0];
        while(--delete_level >= 0)
        {
            *update[delete_level] = current[0]->forward[delete_level];
        }
        delete node;
    }

    delete []update;
}

template<typename Key, typename Value>
typename skiplist<Key, Value>::iterator skiplist<Key, Value>::begin()
{
    return iterator(forward[0]);
}

template<typename Key, typename Value>
typename skiplist<Key, Value>::iterator skiplist<Key, Value>::last()
{
    return iterator(nullptr);
}

template<typename Key, typename Value>
typename skiplist<Key, Value>::const_iterator
skiplist<Key, Value>::cbegin() const
{
    return const_iterator(forward[0]);
}

template<typename Key, typename Value>
typename skiplist<Key, Value>::const_iterator
skiplist<Key, Value>::clast() const
{
    return const_iterator(nullptr);
}

#endif
