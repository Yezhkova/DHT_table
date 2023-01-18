#include "bucket.h"

std::map<ID, NodeInfo>::iterator Bucket::find_node(const Node & node)
{
    return m_bucket.find(node.get_node_id());
}

size_t Bucket::size()
{
    return m_bucket.size();
}

bool Bucket::is_full()
{
    return m_bucket.size() > m_max_bucket_size;
}

bool Bucket::add_node(const Node & new_node)
{
    if(!is_full() && !contains_node(new_node)) // do we check here or elsewhere?
    {
        m_bucket[new_node.get_node_id()] = new_node.get_node_info();
        return true;
    }
    return false;
}

bool Bucket::contains_node(const Node & node)
{
    return find_node(node) != m_bucket.end();
}

bool Bucket::remove_node(const Node & node)
{
    if (Bucket::contains_node(node))
    {
        m_bucket.erase(find_node(node));
        return true;
    }
    return false;
}

