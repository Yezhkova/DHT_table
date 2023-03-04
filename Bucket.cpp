#include "Bucket.h"

size_t Bucket::m_maxBucketSize = 20;

const std::set<Contact> Bucket::data() const {
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

bool Bucket::addNode(const ID& id) {
    return m_Bucket.insert(Contact{id}).second;
}

bool Bucket::containsNode(const ID& id) const {
    return m_Bucket.find(id) != m_Bucket.end();
}

size_t Bucket::removeNode(const ID& id) {
    return m_Bucket.erase(id);
}

std::ostream& operator<<(std::ostream& out, const Bucket& b)
{
    for(auto& e: b.data())
    {
        out << e << ' ';
    }
    out << '\n';
    return out;
}



