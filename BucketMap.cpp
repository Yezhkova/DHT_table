#include "BucketMap.h"
#include "Node.h"
#include "Utils.h"

const std::map<BucketIndex, Bucket>& BucketMap::map() const {
    return m_Buckets;
};

int16_t BucketMap::calcBucketIndex(const ID& id) {
    return m_node.id() == id ? -1 : DIGEST - 1 - m_node.id().prefixLength(id);
}

bool BucketMap::addNode(const Contact& contact)
{
    size_t BucketIndex = calcBucketIndex(contact.id());
    return m_Buckets[BucketIndex].addNode(contact);
}

bool BucketMap::addNode(const ID& id)
{
    size_t BucketIndex = calcBucketIndex(id);
    return m_Buckets[BucketIndex].addNode(Contact(id));
}

bool BucketMap::containsNode(const Contact &contact) const
{
    for(auto & bucket : m_Buckets)
    {
        if(bucket.second.containsNode(contact)) {
            return true;
        }
    }
    return false;
}

bool BucketMap::containsNode(const ID& id) const
{
    for(auto& bucket : m_Buckets)
    {
        if(bucket.second.containsNode(id)) {
            return true;
        }
    }
    return false;
}

std::vector<Bucket> BucketMap::nonEmptyBuckets()
{
    std::vector<Bucket> res;
    for(auto & bucket : m_Buckets) {
        if(bucket.second.size() > 0) {
            res.push_back(bucket.second);
        }
    }
    return res;
}

std::optional<Bucket> BucketMap::getNodesAtDepth(size_t depth) const
{
    try {
       return m_Buckets.at(depth);
    } catch (std::out_of_range) {
        return {};
    }
}

const size_t BucketMap::size() const
{
    return m_Buckets.size();
}

std::ostream& operator<< (std::ostream& out, const BucketMap& b)
{
    LOG(b.m_node.id() << " bucket map size: " << b.m_Buckets.size());
    for(auto& e: b.m_Buckets) {
        out << "Bucket depth " << e.first << ":\n";
        for(auto& e2: e.second.bucket()){
            out << e2.id() << "\n";
        }
    }
    return out;
}


