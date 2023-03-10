#include "Node.h"
#include "Swarm.h"
#include "Utils.h"
#include <optional>

#define CLOSEST_NODES 3
#define FIND_NODE_THRESHOLD 1000
#define PING_THRESHOLD 2
#define PING_INTERVAL 15
#define MINUTES 60000

size_t Node::m_treeSize = 160;
std::mt19937 Node::m_randomGenerator;


const ID& Node::id() const{
    return m_contact.m_id;
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

uint64_t Node::label() const {
    return m_index;
}

void Node::setLabel(uint64_t label) {
    m_index = label;
}
// TODO: node offline mode not implemented

bool Node::addNode(const ID& newId) {
    if (this->id() != newId) {
        int bucketIdx = m_BucketMap.calcBucketIndex(newId);
        if(!m_BucketMap.bucketFull(bucketIdx)){
            bool res = m_BucketMap.addNode(newId, bucketIdx);
            m_protocol.sendPing(newId);
            return res;
        }
        return false;
    }
    return false;
}

bool Node::removeNode(const ID& id) {
    return m_BucketMap.removeNode(id);
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

IDhtTransportProtocol& Node::protocol() {
    return m_protocol;
}

const ID Node::pickRandomNode(const std::set<Contact>& s) const
{
    auto it = s.begin();
    std::uniform_int_distribution<int> range(0, s.size()-1);
    int randomNodeNumber = range(m_randomGenerator);
    std::advance(it, randomNodeNumber);
    return it->m_id;
}

void Node::fill(int idx, std::vector<ID>& ids, int k, const ID& id)
{
    std::set<Contact> bucket = m_BucketMap.data()[idx];
    if(bucket.size() == 0) return;
    std::pair<const ID*, int> candidates [k];

    for(auto& ref : candidates) {
        ref.first = nullptr;
        ref.second = INT_MAX;
    }

//    for(int n = 0; n < k; ++n) {
        for(auto& contact : bucket) {
            int distance = 159 - id.prefixLength(contact.m_id);
            for(int i = 0; i < k; ++i) {
                if(candidates[i].second > distance) {
                    for(int j = k-1; j > i; --j) {
                        candidates[j] = candidates[j-1];
                    }
                    candidates[i].first = &contact.m_id;
                    candidates[i].second = distance;
                    break;
                }
            }
        }
//    }

    for(auto& candidate : candidates) {
        if(candidate.first != nullptr) {
            ids.push_back(*candidate.first);
        }
    }
}

std::vector<ID> Node::findClosestNodes(int k, const ID& id)
{
    std::vector<ID> res;

    // start with the bucket where ID could be
    int bucketIndex = m_BucketMap.calcBucketIndex(id);
    fill(bucketIndex, res, k, id);

    // not enough ids
    if(res.size() < k) {
        int nextBucketIndex = bucketIndex, prevBucketIndex = bucketIndex;
        size_t i = 1;
        for(; nextBucketIndex < m_treeSize && prevBucketIndex >= 0; ++i)
        {
            nextBucketIndex = bucketIndex + i;
            prevBucketIndex = bucketIndex - i;
            fill(nextBucketIndex, res, k, id);
            fill(prevBucketIndex, res, k, id);
        }
        for(size_t j = i; res.size() < k && nextBucketIndex < m_treeSize; ++j)
        {
            nextBucketIndex = bucketIndex + j;
            fill(nextBucketIndex, res, k, id);
        }
        for(size_t j = i; res.size() < k && prevBucketIndex >= 0; ++j)
        {
            prevBucketIndex = bucketIndex - j;
            fill(prevBucketIndex, res, k, id);
        }
    }
//    LOG(id << ": found " << res.size() << " closest nodes.");
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

bool Node::receivePing(const ID & requestorId) {
    return m_online;
}

void Node::receivePingResponse(bool online
                              , const ID & queriedId)
{
    if(auto it = m_pingMap.find(queriedId); it == m_pingMap.end()) {
        return;
    }

    if(!online && m_pingMap[queriedId] < PING_THRESHOLD)
    {
        m_protocol.sendPing(queriedId);
    }
    else {
        onPingEnd(online, queriedId);
    }
}

void Node::receiveFindNodeResponse(const ID& queriedId
                                    , std::vector<ID> ids
                                    , const ID& responserId)
{
    if(auto it = m_findNodeMap.find(queriedId); it == m_findNodeMap.end()) {
        return;
    }
    if (m_findNodeMap[queriedId] > FIND_NODE_THRESHOLD) {
        onFindNodeEnd(false, queriedId);
        return;
    }
    if(ids.size() == 0) {
        return;
    }
    if (ids[0] == queriedId) {
        onFindNodeEnd(true, queriedId);
    }

    else {
        //LOG(queriedId << " not found");
        for (auto& id : ids) {
            m_protocol.sendFindNode(id, this->id(), queriedId); // Swarm does this
        }
    }

}

void Node::onFindNodeStart(const ID& queriedId)
{
    m_findNodeMap[queriedId]++;
}

void Node::onFindNodeEnd(bool found, const ID& queriedId)
{
    m_findNodeMap.erase(queriedId);
    if(found){

    }
    else {
        m_eventHandler.onNodeNotFound();
    }
}

void Node::onPingStart(const ID& queriedId)
{
    m_pingMap[queriedId]= 1;
}

void Node::onPingEnd(bool online, const ID& queriedId)
{
    m_pingMap.erase(queriedId);
    if(!online) {
        removeNode(queriedId);
    }
    m_timerProtocol.startTimer(PING_INTERVAL * MINUTES, [this, queriedId]{
        m_protocol.sendPing(queriedId);
    });
}

void Node::setOffline() {
    m_online = false;
}

void Node::setOnline() {
    m_online = true;
}


