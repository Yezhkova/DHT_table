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
    assert((void("Node shouldn't add itself into its bucketMap"),
            m_contact.id() != id));
    return m_BucketMap.addNode(id);
}

void Node::updateNode(const ID& id)
{
    Swarm::getInstace().getPeer(id)
            ->node().nodeInfo().
            updateLastSeen(system_clock::now());
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
    LOG("-->findClosestNodes...");
    std::vector<ID> res;
    uint16_t bucketIndex = m_BucketMap.calcBucketIndex(id);
    auto closestBucket = m_BucketMap.getNodesAtDepth(bucketIndex);
    fill(closestBucket, res);
    if(res.size() < k) {
        int nextBucketIndex = bucketIndex, prevBucketIndex = bucketIndex;
        for(size_t i = 1;
            res.size() < k && nextBucketIndex < m_treeSize && prevBucketIndex >= 0;
            ++i)
        {
            nextBucketIndex = bucketIndex + i;
            prevBucketIndex = bucketIndex - i;
            auto nextBucket = m_BucketMap.getNodesAtDepth(nextBucketIndex);
            fill(nextBucket, res);
            auto prevBucket = m_BucketMap.getNodesAtDepth(prevBucketIndex);
            fill(prevBucket, res);
        }
        for(size_t i = 1; res.size() < k && nextBucketIndex < m_treeSize; ++i)
        {
            nextBucketIndex = bucketIndex + i;
            auto nextBucket = m_BucketMap.getNodesAtDepth(nextBucketIndex);
            fill(nextBucket, res);
        }
        for(size_t i = 1; res.size() < k && prevBucketIndex >= 0; ++i)
        {
            prevBucketIndex = bucketIndex - i;
            auto prevBucket = m_BucketMap.getNodesAtDepth(prevBucketIndex);
            fill(prevBucket, res);
        }
    }
    LOG("found " << res.size() << " closest nodes.");
    return res;
}


void Node::sendFindNode(const ID & senderId, const ID & queriedId)
{
    receiveFindNode(m_contact.id(), senderId, queriedId);
}

void Node::receiveFindNode(const ID & myID,
                           const ID & senderId, const ID & queriedId)
{
    LOG(myID << " receives FindNode.");
    if(m_BucketMap.containsNode(queriedId))
    {
        sendFindNodeResponse(senderId, myID, queriedId);
//        addNode(senderId);
        updateNode(queriedId);
    }
    else
    {
        std::vector<ID> closestNodes = findClosestNodes(3, queriedId);
        for(auto& id : closestNodes)
        {
            Swarm::getInstace().getPeer(id)->node()
                    .sendFindNode(senderId, queriedId);

            Swarm::getInstace().getPeer(id)->node()
                    .addNode(senderId);
        }

    }
    bool add = addNode(senderId);
    LOG(id() << " just added " << senderId << (add ? "yes" : "no"));
}

void Node::sendFindNodeResponse(const ID & recipientId,
                                const ID & myId, const ID & queriedId)
{
    Swarm::getInstace().getPeer(recipientId)->
            receiveFindNodeResponse(myId, queriedId);
}


