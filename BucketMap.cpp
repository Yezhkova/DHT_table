#include "BucketMap.h"
#include "Node.h"
#include "Utils.h"

uint16_t BucketMap::g_bucketSize = 30;

const std::map<BucketIndex, Bucket>& BucketMap::map() const {
    return m_Buckets;
};

int16_t BucketMap::calcBucketIndex(const ID& id) {
    return m_node.id() == id ? -1 : DIGEST - 1 - m_node.id().prefixLength(id);
}

int16_t BucketMap::bucketSize(int16_t bucketIdx) {
    auto b = getNodesAtDepth(bucketIdx);
    return b.size();
}

bool BucketMap::bucketFull(int16_t bucketIdx) {
    auto b = getNodesAtDepth(bucketIdx);
    return b.size() == g_bucketSize;
}

bool BucketMap::addNode(const ID& id, int16_t BucketIndex) {
    return m_Buckets[BucketIndex].insert(Contact{id}).second;
}

bool BucketMap::removeNode(const ID& id) {
    size_t BucketIndex = calcBucketIndex(id);
    return m_Buckets[BucketIndex].erase(id);
}

bool BucketMap::containsNode(const ID& id) const
{
    for(auto& bucket : m_Buckets)
    {
        Bucket b = bucket.second;
        if(b.find(id) != b.end()) {
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

Bucket BucketMap::getNodesAtDepth(size_t depth) {
   return m_Buckets[depth];
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
        for(auto& e2: e.second){
            out << e2.m_id << "\n";
        }
    }
    return out;
}


