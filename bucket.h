#pragma once
#include "ID.h"
#include "nodeinfo.h"
#include <map> // key = ID, value = lastseen time and others  - STRUCT NodeInfo

class Node;

class Bucket
{
private:
    std::map<ID, NodeInfo>  m_bucket;
    size_t                  m_depth = 0;
    size_t                  m_max_bucket_size = 20;
    bool                    m_empty = true;

public:
    Bucket() = delete;
    Bucket(size_t depth) : m_depth(depth) {};
    Bucket(size_t depth, size_t size) : m_depth(depth),
                                        m_max_bucket_size(size) {};

    std::map<ID, NodeInfo>::iterator find_node(const Node & node);
    bool add_node(const Node & new_node);
    bool contains_node(const Node & node);
    bool remove_node(const Node & node);
    bool is_full();
    size_t size();

};
