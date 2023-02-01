#include "BucketMap.h"

const std::map<BucketIndex, Bucket>& BucketMap::map() {
    return m_Buckets;
};

size_t BucketMap::calcBucketIndex(const ID& id)
{
    // if prefixLength == 160, gives -1
    return DIGEST - 1 - m_id.prefixLength(id);
}

bool BucketMap::addNode(const Contact& contact)
{
    size_t BucketIndex = calcBucketIndex(contact.id());
    return m_Buckets[BucketIndex].addNode(contact);
}

bool BucketMap::addNode(const ID& id)
{
    size_t BucketIndex = calcBucketIndex(id);
    return m_Buckets[BucketIndex].addNode(Contact(id));
}

bool BucketMap::containsNode(const Contact &contact)
{
    for(auto & bucket : m_Buckets)
    {
        if(bucket.second.containsNode(contact)) {
            return true;
        }
    }
    return false;
}

bool BucketMap::containsNode(const ID& id)
{
    for(auto & bucket : m_Buckets)
    {
        if(bucket.second.containsNode(id)) {
            return true;
        }
    }
    return false;
}

std::vector<Bucket> BucketMap::nonEmptyBuckets()
{
    std::vector<Bucket> res;
    for(auto & bucket : m_Buckets) {
        if(bucket.second.size() > 0) {
            res.push_back(bucket.second);
        }
    }
    return res;
}

std::optional<Bucket> BucketMap::getNodesAtDepth(size_t depth)
{
    try
    {
        return m_Buckets.at(depth);
    }
    catch (std::out_of_range &e)
    {
        return {};
    }
}

size_t BucketMap::size()
{
    return m_Buckets.size();
}

