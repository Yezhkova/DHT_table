#include "bucket_list.h"

bool bucket_array::add_node(const node & new_node, ID & id)
{
    uint16_t bucket_index = DIGEST - 1 - id.prefix_length(new_node.get_node_id());
    return m_buckets[bucket_index].add_node(new_node);
}
