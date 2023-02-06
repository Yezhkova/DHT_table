#include "BucketMap.h"
#include "Node.h"
#include "Utils.h"

const std::map<BucketIndex, Bucket>& BucketMap::map() const {
    return m_Buckets;
};

size_t BucketMap::calcBucketIndex(const ID& id)
{
    // if prefixLength == 160, gives -1
    return DIGEST - 1 - m_node.id().prefixLength(id);
}

bool BucketMap::addNode(const Contact& contact)
{
    size_t BucketIndex = calcBucketIndex(contact.id());
    return m_Buckets[BucketIndex].addNode(contact);
}

bool BucketMap::addNode(const ID& id)
{
    size_t BucketIndex = calcBucketIndex(id);
    bool res = m_Buckets[BucketIndex].addNode(Contact(id));
    LOG(m_Buckets.size());
    return res;
}

bool BucketMap::containsNode(const Contact &contact)
{
    for(auto & bucket : m_Buckets)
    {
        if(bucket.second.containsNode(contact)) {
            return true;
        }
    }
    return false;
}

bool BucketMap::containsNode(const ID& id)
{
    LOG("-->containsNode. ");
    LOG(m_Buckets.size());
    for(auto& bucket : m_Buckets)
    {
        LOG("111");
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

const Bucket& BucketMap::getNodesAtDepth(size_t depth) const
{
    return m_Buckets.at(depth);
}

size_t BucketMap::size()
{
    return m_Buckets.size();
}

std::ostream& operator<< (std::ostream& out, const BucketMap& b)
{
    LOG("size: " << b.m_Buckets.size());
    for(auto& e: b.m_Buckets) {
        out << "Bucket depth " << e.first << ":\n";
        for(auto& e2: e.second.bucket()){
            out << e2.id() << "\n";
        }
    }
    return out;
}


