#include "Bucket.h"
#include "Swarm.h"

size_t Bucket::m_maxBucketSize = 20;

const std::list<Contact> &Bucket::bucket() const {
    return m_Bucket;
};

std::list<Contact>::iterator Bucket::find_node(const ID& id)
{
    auto it = m_Bucket.begin();
    for(; it != m_Bucket.end(); ++it) {
        if(*it == id) {
            return it;
        }
    }
    return m_Bucket.end();
}

size_t Bucket::size() const
{
    return m_Bucket.size();
}

bool Bucket::isFull() const
{
    return m_Bucket.size() == m_maxBucketSize;
}

bool Bucket::isEmpty() const
{
    return m_Bucket.empty();
}

// documentation p.5, 2.2
bool Bucket::updateNode(const Contact &contact)
{
// TODO: moving a node to head optimization
// TODO: how many dead nodes do we evict?
    if(!isFull())
    {
        auto it = find_node(contact.id());
        if(it != m_Bucket.end()) {
            m_Bucket.erase(it);
        }
        m_Bucket.push_back(contact);
        return true;
    }
//    ID me = m_bucketMap.m_node.id();
//    Swarm::getInstace().getPeer(me)->sendPing(m_Bucket.front().id());
//    if(!Swarm::getInstace().getPeer(m_Bucket.front().id())->receivePingResponse(me))
//    {
//           m_Bucket.pop_front();
//           m_Bucket.push_back(contact);
//    }
    return false;
}

bool Bucket::addNode(const ID& id)
{
    if(!isFull())
    {
        auto it = find_node(id);
        if(it != m_Bucket.end()) {
            m_Bucket.erase(it);
        }
        m_Bucket.push_front(Contact{id}); //TODO: Contact(id) - difference?
        return true;
    }
    return false;
}

bool Bucket::containsNode(const Contact &contact)
{
    return find_node(contact.id()) != m_Bucket.end();
}

bool Bucket::containsNode(const ID& id)
{
    return find_node(id) != m_Bucket.end();
}

bool Bucket::removeNode(const Contact &contact)
{
    auto it = find_node(contact.id());
    if (it != m_Bucket.end())
    {
        m_Bucket.erase(it);
        return true;
    }
    return false;
}

bool Bucket::removeNode(const ID& id)
{
    auto it = find_node(id);
    if (it != m_Bucket.end())
    {
        m_Bucket.erase(it);
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const Bucket& b)
{
    for(auto& e: b.bucket())
    {
        out << e << ' ';
    }
    out << '\n';
    return out;
}



