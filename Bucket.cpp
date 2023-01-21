#include "Bucket.h"

std::map<ID, NodeInfo>::iterator Bucket::find_Node(const ID &id)
{
    return m_Bucket.find(id);
}

size_t Bucket::size()
{
    return m_Bucket.size();
}

bool Bucket::isFull()
{
    return m_Bucket.size() > m_maxBucketSize;
}

bool Bucket::isEmpty()
{
    return m_Bucket.size() == 0;
}

bool Bucket::addNode(const ID &id, const NodeInfo &Node_info)
{
    if(!isFull() && !containsNode(id)) // do we check here or elsewhere?
    {
        m_Bucket[id] = Node_info;
        return true;
    }
    return false;
}

bool Bucket::containsNode(const ID &id)
{
    return find_Node(id) != m_Bucket.end();
}

bool Bucket::removeNode(const ID &id)
{
    auto it = find_Node(id);
    if (it != m_Bucket.end())
    {
        m_Bucket.erase(it);
        return true;
    }
    return false;
}

