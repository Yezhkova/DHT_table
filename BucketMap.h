#pragma once

#include "Bucket.h"
#include <map>
#include <optional>

using BucketIndex = size_t;
class Node;
class BucketMap
{
private:
    ID                              m_id;
    std::map<BucketIndex, Bucket>   m_Buckets;

public:
    BucketMap() = default;
    BucketMap(const ID & id) : m_id(id) {};

    bool addNode(const ID & id, const Node* node);
    bool hasNode(const ID & id);
    std::map<BucketIndex, Bucket> map() { return m_Buckets; };
    std::optional<Bucket> getNodesAtDepth(size_t depth);
    size_t size();

    std::vector<Bucket> nonEmptyBuckets();

    size_t calcBucketIndex(const ID & id);

};
