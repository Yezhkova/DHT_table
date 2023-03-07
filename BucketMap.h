#pragma once

//#include "Bucket.h"
#include "Contact.h"
#include <map>
#include <set>

class Node;
using BucketIndex = size_t;
using Bucket = std::set<Contact>;

class BucketMap
{
private:

    Node&                           m_node;
    std::map<BucketIndex, Bucket>   m_Buckets;
    static uint16_t                 g_bucketSize;

public:

    BucketMap() = delete;
    BucketMap(Node& node) : m_node(node) {};

    bool addNode(const ID& id, int16_t BucketIndex);
    bool removeNode(const ID& id);
    bool containsNode(const ID& id) const;

    const std::map<BucketIndex, Bucket>& map() const;
    Bucket getNodesAtDepth(size_t depth);
    const size_t size() const;
    int16_t bucketSize(int16_t bucketIdx);
    bool bucketFull(int16_t bucketIdx);

    std::vector<Bucket> nonEmptyBuckets();

    int16_t calcBucketIndex(const ID& id);
    friend std::ostream& operator<< (std::ostream& out, const BucketMap& b);

};
