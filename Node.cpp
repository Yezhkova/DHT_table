#include "Node.h"
#include "Utils.h"
#include "Swarm.h"
#include <optional>

#define CLOSEST_NODES 3
#define FIND_NODE_TIMEOUT 10000
#define PING_THRESHOLD 1

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

uint64_t Node::label() const {
    return m_label;
}

void Node::setLabel(uint64_t label){
    m_label = label;
}

bool Node::addNode(const ID& newId) {
    if (this->id() != newId) {
        int16_t bucketIdx = m_BucketMap.calcBucketIndex(newId);
        if(!m_BucketMap.bucketFull(bucketIdx)){
            return m_BucketMap.addNode(newId, bucketIdx);
        }
        int16_t size = m_BucketMap.bucketSize(bucketIdx);
        for(auto& contact : m_BucketMap.getNodesAtDepth(bucketIdx).value().data())
        {
            m_protocol.sendPing(contact.id());
        }
        if(m_BucketMap.bucketSize(bucketIdx) < size) {
            addNode(newId);
        }
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

IKademliaTransportProtocol& Node::protocol() {
    return m_protocol;
}

const ID Node::pickRandomNode(const std::set<Contact>& s) const
{
    auto it = s.begin();
    std::uniform_int_distribution<int> range(0, s.size()-1);
    int randomNodeNumber = range(m_randomGenerator);
    std::advance(it, randomNodeNumber);
    return it->id();
}

void Node::fill(int idx, std::vector<ID>& ids, int k)
{
    auto bucket = m_BucketMap.getNodesAtDepth(idx);
    if(bucket.has_value()) {
        for(uint16_t i = 0; i < k; ++i) {
            ids.push_back(pickRandomNode(bucket.value().data()));
            // TODO: в бакете ~3 ноды и он все три раза (случайно) выбрал одну и ту же
            // или ноды 1, 2, 2 . (ЭТО РЕАЛЬНО) тогда какой смысл
        }
        // это медленно на втором этапе (рандомные пары)

//        for(auto& contact : bucket.value().data()) {
//            ids.push_back(contact.id());
//        }
        // тут медленнее бутстрап, но гораздо быстрее 2 фаза
    }
}

std::vector<ID> Node::findClosestNodes(int k, const ID & id)
{
    std::vector<ID> res;

    // start with the bucket where ID could be
    uint16_t bucketIndex = m_BucketMap.calcBucketIndex(id);
    fill(bucketIndex, res, k);

    // not enough ids
    if(res.size() < k) {
        int nextBucketIndex = bucketIndex, prevBucketIndex = bucketIndex;
        size_t i = 1;
        for(; nextBucketIndex < m_treeSize && prevBucketIndex >= 0; ++i)
        {
            nextBucketIndex = bucketIndex + i;
            prevBucketIndex = bucketIndex - i;
            fill(nextBucketIndex, res, k);
            fill(prevBucketIndex, res, k);
        }
        for(size_t j = i; res.size() < k && nextBucketIndex < m_treeSize; ++j)
        {
            nextBucketIndex = bucketIndex + j;
            fill(nextBucketIndex, res, k);
        }
        for(size_t j = i; res.size() < k && prevBucketIndex >= 0; ++j)
        {
            prevBucketIndex = bucketIndex - j;
            fill(prevBucketIndex, res, k);
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

    if (m_findNodeMap[queriedId] > FIND_NODE_TIMEOUT) {
        Swarm::getInstance().getPeer(queriedId)->PeerStatistics::setFailedFindNode();
        onFindNodeEnd(false, queriedId);
    }
    else if (ids[0] == queriedId) {
        Swarm::getInstance().getPeer(queriedId)->PeerStatistics::incReceiveFindNodeCounter();
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
        if(m_label == Swarm::getInstance().peers().size()) {
            // add onBootstrap task
            Swarm::getInstance().addTaskAfter(0, [] {
                Swarm::getInstance().calculateStatistic();
                Swarm::getInstance().eventQueqe().removeAllEvents();
                auto peers = Swarm::getInstance().peers();
                for (auto& peer : peers)
                {
                    peer.second->resetFindNodeCounter();
                    peer.second->resetPacketCounter();
                    peer.second->resetReceiveFindNodeCounter();
                    peer.second->resetFailedNode();
                }
                for (auto& peer : peers)
                {
                    peer.second->onBootstrap();
                }
            });
        }
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
    m_timerProtocol.startTimer(15*60000, [this, queriedId]{
        m_protocol.sendPing(queriedId);
    });
}

void Node::setOffline() {
    m_online = false;
}

void Node::setOnline() {
    m_online = true;
}


