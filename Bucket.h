#pragma once
#include "ID.h"
#include "NodeInfo.h"
#include <map> // key = ID, value = lastseen time and others  - STRUCT NodeInfo

class Node;

class Bucket
{
private:
    std::map<ID, NodeInfo>  m_Bucket;
    size_t                  m_depth = 0;
    size_t                  m_maxBucketSize = 20;
    bool                    m_empty = true;

public:
    Bucket() = default;
    Bucket(size_t depth) : m_depth(depth) {};
    Bucket(size_t depth, size_t size) : m_depth(depth),
                                        m_maxBucketSize(size) {};

    std::map<ID, NodeInfo>::iterator find_Node(const ID &id);
    bool addNode(const ID &id, const NodeInfo &Node_info);
    bool containsNode(const ID &id);
    bool removeNode(const ID &id);
    bool isFull();
    bool isEmpty();
    size_t size();

};
