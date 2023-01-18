#pragma once

#include "bucket.h"
#include <map>

using BucketIndex = uint16_t;
class Node;
class BucketMap
{
private:
    ID                              m_id;
    std::map<BucketIndex, Bucket>   m_buckets;
    size_t                          m_size = 160;

public:
    BucketMap() = delete;
    BucketMap(const ID & id) : m_id(id) {};
    BucketMap(const ID & id, size_t size) : m_id(id), m_size(size) {};

    bool addNode(const Node & new_node);
    bool hasNode(const Node & node);

    size_t calc_bucket_index(ID id);
};

// camelStyle = me, snake_style = STL
