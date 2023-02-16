#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"
#include <boost/chrono.hpp>

std::mt19937 Peer::s_randomGenerator;

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
    std::uniform_int_distribution<int> range(1, 100);
    m_packetTime = range(s_randomGenerator) / 1000; // random number from 10 to 100 ms; data speed
    addNode(bootstrapId);
    sendFindNode(bootstrapId, m_node.id(),  m_node.id());

    // addtask():
    // pick random peer from Swarm with random delay
    // findNode(randomPeer)
    Swarm::getInstance().addTask(0.002, [&range, this] {

        for(int i = 0; i < 10; ++i) {
            double delay = range(s_randomGenerator) / 1000;
            auto it = Swarm::getInstance().peers().begin();
            std::uniform_int_distribution<int> range(0
                            , Swarm::getInstance().peers().size()-1);
            int randomPeerNumber = range(s_randomGenerator);
            std::advance(it, randomPeerNumber);
            sendPing(it->second->id());
        }
    });
}

void Peer::sendPing(const ID& queriedId) {
    Swarm::getInstance().addTask(0.002, [this, queriedId] {
        auto recipient = Swarm::getInstance().getPeer(queriedId);
        if(recipient != nullptr) {
            recipient->receivePing(m_node.id());
        }
        else{
            LOG("sendPing Warning: the recipient peer does not exist");
        }
    });
}

void Peer::receivePing(const ID & queryingId) {
    Swarm::getInstance().addTask(0.002, [&queryingId, this] {
        Swarm::getInstance().getPeer(queryingId)
               ->receivePingResponse(m_node.id());
    });
}

bool Peer::receivePingResponse(const ID& queriedId) {
    Swarm::getInstance().addTask(0.002, [&queriedId] {
        LOG(queriedId << " is alive!");
    });
    return true;
}

void Peer::sendFindNode(const ID& recipientId
                        , const ID& senderId
                        , const ID& queriedId) {
    // sender side
    Swarm::getInstance().addTask(0.002, [recipientId, queriedId, senderId] {
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
    // receiver side)
    auto idsFound = m_node.receiveFindNode(senderId, queriedId
                                           , boost::chrono::system_clock::now());
    sendFindNodeResponse(senderId, queriedId, idsFound);
}

void Peer::sendFindNodeResponse(const ID& receiverId, const ID& queriedId, std::vector<ID> idsFound) {
    // receiver side
    Swarm::getInstance().addTask(0.003, [receiverId, queriedId, idsFound, this] {
        auto receiver = Swarm::getInstance().getPeer(receiverId);
        if(receiver != nullptr) {
            receiver->receiveFindNodeResponse(queriedId, idsFound, id());
        }
        else {
            LOG("sendFindNodeResponse Warning: the recipient peer does not exist");
        }
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




void Peer::onFindNodeResponse(bool find, int packetNumber)
{

}
void Peer::onPacketReceived()
{

}
void Peer::onPacketSent()
{

}

