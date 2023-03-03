#include "Node.h"
#include "Utils.h"
#include "Swarm.h"
#include <optional>

#define CLOSEST_NODES 3
#define TIMEOUT 1000

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

INodeEventHandler& Node::eventHandler() {
    return m_eventHandler;
}

void Node::randomizeId() {
    m_contact.randomize();
}

bool Node::addNode(const ID& id) {
    if (this->id() != id) {
        return m_BucketMap.addNode(id);
    }
    return false;
}

void Node::updateLastSeen(const ID& id
                          , boost::chrono::system_clock::time_point time) {
    auto peerToUpdate = Swarm::getInstance().getPeer(id);
    if(peerToUpdate != nullptr) {
        peerToUpdate->info().updateLastSeen(time);
    }
}

bool operator==(const Node& l, const Node& r) {
    return l.m_contact.m_id == r.m_contact.m_id;
}

IKademliaTransportProtocol& Node::protocol() {
    return m_protocol;
}

const ID& Node::pickRandomNode(const Bucket& b) const
{
    auto it = b.bucket().begin();
    std::uniform_int_distribution<int> range(0, b.size()-1);
    int randomNodeNumber = range(m_randomGenerator);
    std::advance(it, randomNodeNumber);
    return it->id();
}

void Node::fill(std::optional<Bucket>& bucket, std::vector<ID>& ids, int k)
{
    if(bucket.has_value()) {
        for(uint16_t i = 0; i < k; ++i) {
            ids.push_back(pickRandomNode(bucket.value()));
            // TODO: в бакете ~3 ноды и он все три раза (случайно) выбрал одну и ту же  
            // или ноды 1, 2, 2 . (ЭТО РЕАЛЬНО) тогда какой смысл
        }

// TODO: THIS is simpler, is this better?

//        for(auto& contact: bucket.value().bucket()) {
//            ids.push_back(contact.id());
//        }
    }
}

std::vector<ID> Node::findClosestNodes(int k, const ID & id)
{
    //TODO: do we return exactly k closest nodes or AT LEAST k?

    std::vector<ID> res;

    // start with the bucket where ID could be
    uint16_t bucketIndex = m_BucketMap.calcBucketIndex(id);
    auto closestBucket = m_BucketMap.getNodesAtDepth(bucketIndex);
    fill(closestBucket, res, k);

    // not enough ids
    if(res.size() < k) {
        int nextBucketIndex = bucketIndex, prevBucketIndex = bucketIndex;
        size_t i = 1;
        for(; nextBucketIndex < m_treeSize && prevBucketIndex >= 0; ++i)
        {
            nextBucketIndex = bucketIndex + i;
            prevBucketIndex = bucketIndex - i;
            auto nextBucket = m_BucketMap.getNodesAtDepth(nextBucketIndex);
            fill(nextBucket, res, k);
            auto prevBucket = m_BucketMap.getNodesAtDepth(prevBucketIndex);
            fill(prevBucket, res, k);
        }
        for(size_t j = i; res.size() < k && nextBucketIndex < m_treeSize; ++j)
        {
            nextBucketIndex = bucketIndex + j;
            auto nextBucket = m_BucketMap.getNodesAtDepth(nextBucketIndex);
            fill(nextBucket, res, k);
        }
        for(size_t j = i; res.size() < k && prevBucketIndex >= 0; ++j)
        {
            prevBucketIndex = bucketIndex - j;
            auto prevBucket = m_BucketMap.getNodesAtDepth(prevBucketIndex);
            fill(prevBucket, res, k);
        }
    }
    //LOG(id << ": found " << res.size() << " closest nodes.");
    return res;
}

std::vector<ID> Node::receiveFindNode(const ID& senderId
                                      , const ID& queriedId)
{   
    addNode(senderId);
    if(m_BucketMap.containsNode(queriedId)) {
        return {queriedId};
    }
    else {
        return findClosestNodes(CLOSEST_NODES, queriedId);
    }
}

void Node::sendFindNodeResponse(const ID & recipientId,
                                const ID & myId, const ID & queriedId) {

}

void Node::sendPing(const ID & queryingId) {
    sendPingResponse(queryingId);
}

void Node::sendPingResponse(const ID & queryingId) {
    Swarm::getInstance().getPeer(queryingId)->receivePingResponse(id());
}

void Node::receiveFindNodeResponse(const ID& queriedId
                                    , std::vector<ID> ids
                                    , const ID& responserId)
{
    if (Swarm::getInstance().getPeer(queriedId)->PeerStatistics::findNode() > TIMEOUT) {
        Swarm::getInstance().getPeer(queriedId)->PeerStatistics::setFailedFindNode();
        m_eventHandler.onFindNodeResponse(false);
    }
    else if (ids[0] == queriedId) {
        //LOG(queriedId << " found from " << responserId);
        Swarm::getInstance().getPeer(queriedId)->PeerStatistics::incReceiveFindNodeCounter();
        m_eventHandler.onFindNodeResponse(true);
    }
    else {
        //LOG(queriedId << " not found");
        for (auto& id : ids) {
            m_protocol.sendFindNode(id, this->id(), queriedId); // Swarm does this
        }
    }
}


