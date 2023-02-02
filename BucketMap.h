#pragma once

#include "Bucket.h"
#include <map>
#include <optional>

using BucketIndex = size_t;
class BucketMap
{
private:
    ID                              m_id;
    std::map<BucketIndex, Bucket>   m_Buckets;

public:
    BucketMap() = default;
    BucketMap(const ID & id) : m_id(id) {};

    bool addNode(const Contact& contact);
    bool addNode(const ID& id);

    bool containsNode(const Contact& contact);
    bool containsNode(const ID& id);

    const std::map<BucketIndex, Bucket>& map() const;
    const Bucket& getNodesAtDepth(size_t depth) const;
    size_t size();

    std::vector<Bucket> nonEmptyBuckets();

    size_t calcBucketIndex(const ID& id);

};
