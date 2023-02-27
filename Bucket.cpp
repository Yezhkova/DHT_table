#include "Bucket.h"
#include "Swarm.h"

size_t Bucket::m_maxBucketSize = 20;

const std::set<Contact> &Bucket::bucket() const {
    return m_Bucket;
};

std::set<Contact>::iterator Bucket::find_node(const ID& id) {
    return m_Bucket.find(id);
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
bool Bucket::addNode(const Contact &contact)
{
    if(!isFull()) {
       m_Bucket.insert(contact).second;
       return true;
    }
//    ID me = m_bucketMap.m_node.id();
//    Swarm::getInstace().getPeer(me)->sendPing(m_Bucket.front().id());
//     while answering -> ping until failure
//    if(!Swarm::getInstace().getPeer(m_Bucket.front().id())->receivePingResponse(me))
//    {
//           m_Bucket.pop_front();
//           m_Bucket.push_back(contact);
//    }
    return false;
}

bool Bucket::addNode(const ID& id)
{
    if(!isFull()) {
       return m_Bucket.insert(Contact{id}).second;
    }
    return false;
}

bool Bucket::containsNode(const Contact &contact) {
    return m_Bucket.find(contact.id()) != m_Bucket.end();
}

bool Bucket::containsNode(const ID& id) {
    return m_Bucket.find(id) != m_Bucket.end();
}

size_t Bucket::removeNode(const Contact &contact) {
    return m_Bucket.erase(contact.id());
}

size_t Bucket::removeNode(const ID& id) {
    return m_Bucket.erase(id);
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



