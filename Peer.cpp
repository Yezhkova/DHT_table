#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"
#include <boost/chrono.hpp>

ID Peer::id() {
    return m_node.id();
}

Node& Peer::node() {
    return m_node;
}

NodeInfo& Peer::info() {
    return m_node.nodeInfo();
}

void Peer::randomize()
{
    m_node.randomizeId();
}

bool Peer::addNode(const ID & id)
{
    if(m_node.id() != id) {
        return m_node.addNode(id);
    }
    return false;
}

void Peer::start(const ID& bootstrapId)
{
    // task for peer: select some random peers from Swarm and Ping them
    m_packetTime = 0.001; // 1 ms; TODO: random number from 10 to 100 ms; data speed
    addNode(bootstrapId);
    sendFindNode(bootstrapId, m_node.id(),  m_node.id());

    // addtask():
    // pick random peer from Swarm with random delay
    // findNode(randomPeer)
}

void Peer::sendPing(const ID& queriedId) {
    Swarm::getInstance().getPeer(queriedId)->node().sendPing(m_node.id());
}


void Peer::sendFindNode(const ID& recipientId
                        , const ID& senderId
                        , const ID& queriedId)
{
    // sender side
    Swarm::getInstance().addTask([recipientId, queriedId, senderId] {
        auto recipient = Swarm::getInstance().getPeer(recipientId);
        if(recipient != nullptr) {

            recipient->receiveFindNode(senderId, queriedId);
        }
        else{
            LOG("sendFindNode Warning: the recipient peer does not exist");
        }
    });

}

void Peer::receiveFindNode(const ID& senderId
                           , const ID& queriedId) {
    // receiver side
    auto idsFound = m_node.receiveFindNode(senderId, queriedId
                                           , boost::chrono::system_clock::now());
    sendFindNodeResponse(senderId, queriedId, idsFound);
}

void Peer::sendFindNodeResponse(const ID& receiverId, const ID& queriedId, std::vector<ID> idsFound) {
    // receiver side
    Swarm::getInstance().addTask([receiverId, queriedId, idsFound, this] {
        auto receiver = Swarm::getInstance().getPeer(receiverId);
        receiver->receiveFindNodeResponse(queriedId, idsFound, id());
    });
}

void Peer::receiveFindNodeResponse(const ID& queriedId, std::vector<ID> ids, const ID& responserId) {
    if(ids.size() == 1 && ids[0] == queriedId) {
        LOG(queriedId << " found from " << responserId);
    }
    else {
        LOG(queriedId << " not found");
        for(auto& id : ids) {
            sendFindNode(id, m_node.id(),queriedId);
        }
    }
}

bool Peer::receivePingResponse(const ID& queriedId) {
    LOG(queriedId << " is alive!");
    return true;
}


void Peer::onFindNodeResponse(bool find, int packetNumber)
{

}
void Peer::onPacketReceived()
{

}
void Peer::onPacketSent()
{

}

