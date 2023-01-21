#include "BucketMap.h"

size_t BucketMap::calcBucketIndex(const ID & id)
{
    // if prefixLength == 160, gives -1
    return DIGEST - 1 - m_id.prefixLength(id);
}

bool BucketMap::addNode(const ID &id, const NodeInfo &NodeInfo)
{
    uint16_t BucketIndex = calcBucketIndex(id);
    return m_Buckets[BucketIndex].addNode(id, NodeInfo);
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

size_t BucketMap::getSize()
{
    return m_Buckets.size();
}

