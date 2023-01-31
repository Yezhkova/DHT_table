#include "Bucket.h"

std::set<Contact>::iterator Bucket::find_node(const Contact &contact)
{
    return m_Bucket.find(contact);
}

size_t Bucket::size()
{
    return m_Bucket.size();
}

bool Bucket::isFull()
{
    return m_Bucket.size() == m_maxBucketSize;
}

bool Bucket::addNode(const Contact &contact)
{
    if(!isFull())
    {
        m_Bucket.insert(contact);
        return true;
    }
    return false;
}

bool Bucket::containsNode(const Contact &contact)
{
    return find_node(contact) != m_Bucket.end();
}

bool Bucket::removeNode(const Contact &contact)
{
    auto it = find_node(contact);
    if (it != m_Bucket.end())
    {
        m_Bucket.erase(it);
        return true;
    }
    return false;
}

