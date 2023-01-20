#pragma once

#include "bucket.h"
#include <map>
#include <optional>

using BucketIndex = size_t;
class Node;
class BucketMap
{
private:
    ID                              m_id;
    std::map<BucketIndex, Bucket>   m_buckets;

public:
    BucketMap() = default;
    BucketMap(const ID & id) : m_id(id) {};

    bool addNode(const ID & id, const NodeInfo & node_info);
    bool hasNode(const Node & node);
    std::optional<Bucket> getNodesAtDepth(size_t depth);
    size_t getSize();

    size_t calcBucketIndex(const ID & id);
};

// camelStyle = me, snake_style = STL
