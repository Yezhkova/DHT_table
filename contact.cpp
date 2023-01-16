#include "contact.h"

bool contact::add_node(const node & node)
{
    m_bucket_list.add_node(node, m_node.get_node_id());
}
