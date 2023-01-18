#include "bucket_map.h"

size_t BucketMap::calc_bucket_index(ID id)
{
    // if prefix_length == 160, gives -1
    return DIGEST - 1 - m_id.prefix_length(id);
}

bool BucketMap::addNode(const Node & new_node)
{
    uint16_t bucket_index =  calc_bucket_index(new_node.get_node_id());
    return m_buckets[bucket_index].add_node(new_node);
}

