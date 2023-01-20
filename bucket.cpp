#include "bucket.h"

std::map<ID, NodeInfo>::iterator Bucket::find_node(const ID &id)
{
    return m_bucket.find(id);
}

size_t Bucket::size()
{
    return m_bucket.size();
}

bool Bucket::is_full()
{
    return m_bucket.size() > m_max_bucket_size;
}

bool Bucket::is_empty()
{
    return m_bucket.size() == 0;
}

bool Bucket::add_node(const ID &id, const NodeInfo &node_info)
{
    if(!is_full() && !contains_node(id)) // do we check here or elsewhere?
    {
        m_bucket[id] = node_info;
        return true;
    }
    return false;
}

bool Bucket::contains_node(const ID &id)
{
    return find_node(id) != m_bucket.end();
}

bool Bucket::remove_node(const ID &id)
{
    auto it = find_node(id);
    if (it != m_bucket.end())
    {
        m_bucket.erase(it);
        return true;
    }
    return false;
}

