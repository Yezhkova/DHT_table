#include "bucket_map.h"

size_t BucketMap::calcBucketIndex(const ID & id)
{
    // if prefix_length == 160, gives -1
    return DIGEST - 1 - m_id.prefix_length(id);
}

bool BucketMap::addNode(const ID &id, const NodeInfo &node_info)
{
    uint16_t bucket_index = calcBucketIndex(id);
    return m_buckets[bucket_index].add_node(id, node_info);
}

std::optional<Bucket> BucketMap::getNodesAtDepth(size_t depth)
{
    try
    {
        return m_buckets.at(depth);
    }
    catch (std::out_of_range &e)
    {
        return {};
    }
}

size_t BucketMap::getSize()
{
    return m_buckets.size();
}

