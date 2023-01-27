#include "BucketMap.h"

size_t BucketMap::calcBucketIndex(const ID & id)
{
    // if prefixLength == 160, gives -1
    return DIGEST - 1 - m_id.prefixLength(id);
}

bool BucketMap::addNode(const ID &id, const Node* node)
{
    size_t BucketIndex = calcBucketIndex(id);
    return m_Buckets[BucketIndex].addNode(id, node);
}

bool BucketMap::hasNode(const ID & id)
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
        if(!bucket.second.isEmpty())
            res.push_back(bucket.second);
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

