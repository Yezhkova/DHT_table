#pragma once

#include "Bucket.h"
#include <map>

class Node;
using BucketIndex = size_t;
class BucketMap
{
private:
    Node&                           m_node;
    std::map<BucketIndex, Bucket>   m_Buckets;

public:
    BucketMap() = delete;
    BucketMap(Node& node) : m_node(node) {};

    bool addNode(const Contact& contact);
    bool addNode(const ID& id);

    bool containsNode(const Contact& contact);
    bool containsNode(const ID& id);

    const std::map<BucketIndex, Bucket>& map() const;
    const Bucket& getNodesAtDepth(size_t depth) const;
    size_t size();

    std::vector<Bucket> nonEmptyBuckets();

    size_t calcBucketIndex(const ID& id);
    friend std::ostream& operator<< (std::ostream& out, const BucketMap& b);

};
