#pragma once
#include "ID.h"
#include <map>

class Node;

class Bucket
{
private:
    std::map<ID, Node*>  m_Bucket;
    size_t               m_depth = 0;
    size_t               m_maxBucketSize = 20;
    bool                 m_empty = true;

public:
    Bucket() = default;
    Bucket(size_t depth) : m_depth(depth) {};
    Bucket(size_t depth, size_t size) : m_depth(depth),
                                        m_maxBucketSize(size) {};

    std::map<ID, Node*> bucket() { return m_Bucket; };
    std::map<ID, Node*>::iterator find_node(const ID &id);
    bool addNode(const ID &id, const Node* node);
    bool containsNode(const ID &id);
    bool removeNode(const ID &id);
    bool isFull();
    bool isEmpty();
    size_t size();

};
