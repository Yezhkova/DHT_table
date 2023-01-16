#include "bucket.h"

std::_Fwd_list_iterator<node> bucket::find_node(node node)
{
    return std::find(m_bucket.begin(), m_bucket.end(), node);
}

uint16_t bucket::size()
{
    return distance(m_bucket.begin(), m_bucket.end());
}

bool bucket::is_full()
{
    return size() > m_max_bucket_size;
}

bool bucket::add_node(const node & new_node)
{
    if(!is_full() && !contains_node(new_node)) // do we check here or elsewhere?
    {
        m_bucket.push_front(new_node);
        return true;
    }
    return false;
}

bool bucket::contains_node(const node & node)
{
    return find_node(node) != m_bucket.end();
}

bool bucket::remove_node(const node & node)
{
    if (bucket::contains_node(node))
    {
        m_bucket.erase_after(find_node(node));
        return true;
    }
    return false;
}

