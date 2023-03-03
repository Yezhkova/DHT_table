#pragma once

#include "Bucket.h"
#include <map>
#include <optional>

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

    bool containsNode(const Contact& contact)const;
    bool containsNode(const ID& id) const;

    const std::map<BucketIndex, Bucket>& map() const;
    std::optional<Bucket> getNodesAtDepth(size_t depth) const;
    const size_t size() const;

    std::vector<Bucket> nonEmptyBuckets();

    int16_t calcBucketIndex(const ID& id);
    friend std::ostream& operator<< (std::ostream& out, const BucketMap& b);

};
