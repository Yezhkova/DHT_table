#include "Node.h"
#include "Utils.h"
#include "Swarm.h"
#include <optional>

size_t Node::m_treeSize = 160;
std::mt19937 Node::m_randomGenerator;

const ID& Node::id() const{
    return m_contact.id();
}

const BucketMap& Node::bucketMap() const{
    return m_BucketMap;
}

const Contact& Node::contact() const {
    return m_contact;
}

NodeInfo& Node::nodeInfo() {
    return m_info;
}

INodeEventHandler& Node::eventHandler()
{
    return m_eventHandler;
}

void Node::randomizeId()
{
    m_contact.randomize();
}

bool Node::addNode(const ID& id)
{
    return m_BucketMap.addNode(id);
}

void Node::updateLastSeen()
{
    nodeInfo().updateLastSeen(system_clock::now());
}

bool operator==(const Node& l, const Node& r)
{
    return l.m_contact.m_id == r.m_contact.m_id;
}

IKademliaTransportProtocol& Node::protocol()
{
    return m_protocol;
}

const ID& Node::pickRandomNode(const Bucket& b) const
{
    auto it = b.bucket().begin();
    std::uniform_int_distribution<int> range(0, b.size()-1);
//    std::mt19937 m_randomGenerator;
    int randomNodeNumber = range(m_randomGenerator);
    std::advance(it, randomNodeNumber);
    return it->id();
}

void Node::fill(std::optional<Bucket>& bucket, std::vector<ID>& ids)
{
    if(bucket.has_value()) {
        for(auto& contact: bucket.value().bucket()) {
            ids.push_back(contact.id());
        }
    }
}

std::vector<ID> Node::findClosestNodes(uint16_t k, const ID & id)
{
    updateLastSeen();
    LOG("-->findClosestNodes...");
    std::vector<ID> res;
    uint16_t bucketIndex = m_BucketMap.calcBucketIndex(id);
    auto closestBucket = m_BucketMap.getNodesAtDepth(bucketIndex);
    fill(closestBucket, res);
    if(res.size() < k) {
        int nextBucketIndex = bucketIndex, prevBucketIndex = bucketIndex;
        size_t i = 1;
        for(; res.size() < k && nextBucketIndex < m_treeSize && prevBucketIndex >= 0;
            ++i)
        {
            nextBucketIndex = bucketIndex + i;
            prevBucketIndex = bucketIndex - i;
            auto nextBucket = m_BucketMap.getNodesAtDepth(nextBucketIndex);
            fill(nextBucket, res);
            auto prevBucket = m_BucketMap.getNodesAtDepth(prevBucketIndex);
            fill(prevBucket, res);
        }
        for(size_t j = i; res.size() < k && nextBucketIndex < m_treeSize; ++j)
        {
            nextBucketIndex = bucketIndex + j;
            auto nextBucket = m_BucketMap.getNodesAtDepth(nextBucketIndex);
            fill(nextBucket, res);
        }
        for(size_t j = i; res.size() < k && prevBucketIndex >= 0; ++j)
        {
            prevBucketIndex = bucketIndex - j;
            auto prevBucket = m_BucketMap.getNodesAtDepth(prevBucketIndex);
            fill(prevBucket, res);
        }
    }
    LOG("found " << res.size() << " closest nodes.");
    for(auto& e : res){LOG(e);};
    return res;
}


void Node::sendFindNode(const ID & senderId, const ID & queriedId)
{
    updateLastSeen();
    receiveFindNode(id(), senderId, queriedId);
}

void Node::receiveFindNode(const ID & myID,
                           const ID & senderId, const ID & queriedId)
{
    updateLastSeen();
    LOG(myID << " receives FindNode.");
    if(m_BucketMap.containsNode(queriedId))
    {
        sendFindNodeResponse(senderId, myID, queriedId);
        bool add = addNode(senderId);
        LOG(id() << " just added " << senderId << (add ? " yes" : " no"));
    }
    else
    {
        bool add = addNode(senderId);
        LOG(id() << " just added " << senderId << (add ? " yes" : " no"));
        std::vector<ID> closestNodes = findClosestNodes(3, queriedId);
        for(auto& id : closestNodes)
        {
            Swarm::getInstace().getPeer(id)->node()
                    .sendFindNode(senderId, queriedId);

            Swarm::getInstace().getPeer(id)->node()
                    .addNode(senderId);
        }

    }
}

void Node::sendFindNodeResponse(const ID & recipientId,
                                const ID & myId, const ID & queriedId)
{
    updateLastSeen();
    Swarm::getInstace().getPeer(recipientId)->
            receiveFindNodeResponse(myId, queriedId);
}

void Node::sendPing(const ID & queryingId) {
    updateLastSeen();
    sendPingResponse(queryingId);
}

void Node::sendPingResponse(const ID & queryingId) {
    Swarm::getInstace().getPeer(queryingId)->receivePingResponse(id());
}


