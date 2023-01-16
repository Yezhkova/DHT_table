#include "bucket_list.h"

bool bucket_list::add_node(const node & new_node, const ID & id)
{
    uint16_t bucket_index = id.prefix_length(new_node.get_node_id());

}
