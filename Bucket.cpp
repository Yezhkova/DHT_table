#include "Bucket.h"

std::set<Contact>::iterator Bucket::find_node(const ID &id)
{
    return m_Bucket.find(id);
}

size_t Bucket::size()
{
    return m_Bucket.size();
}

bool Bucket::isFull()
{
    return m_Bucket.size() == m_maxBucketSize;
}

bool Bucket::addNode(const ID &id)
{
    if(!isFull())
    {
        m_Bucket.insert(id);
        return true;
    }
    return false;
}

bool Bucket::containsNode(const ID &id)
{
    return find_node(id) != m_Bucket.end();
}

bool Bucket::removeNode(const ID &id)
{
    auto it = find_node(id);
    if (it != m_Bucket.end())
    {
        m_Bucket.erase(it);
        return true;
    }
    return false;
}

