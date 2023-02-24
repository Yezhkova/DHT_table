#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"
#include <boost/chrono.hpp>

#define MILLISEC_IN_SEC 1000.0

std::mt19937 Peer::s_randomGenerator;

Peer::Peer(ID id, IKademliaTransportProtocol& protocol, bool useTcp)
    : m_node(id, protocol, *this) {
    std::uniform_int_distribution<int> range(1, 100);

    // random number from 1 to 100 ms; data speed
    m_packetTime = range(s_randomGenerator) / MILLISEC_IN_SEC;
}

ID Peer::id() {
    return m_node.id();
}

Node& Peer::node() {
    return m_node;
}

NodeInfo& Peer::info() {
    return m_node.nodeInfo();
}

EventQueue::Interval Peer::packetTime() const {
    return m_packetTime;
}

void Peer::randomize() {
    m_node.randomizeId();
}

bool Peer::addNode(const ID & id) {
    if(m_node.id() != id) {
        return m_node.addNode(id);
    }
    return false;
}

void Peer::start(const ID& bootstrapId)
{
    addNode(bootstrapId);
    sendFindNode(bootstrapId, m_node.id(),  m_node.id());

    // task for peer: select some random peers from Swarm and Ping them
    // TODO: findNode or ping?
    // TODO: for(...) {pick and ping} OR fill vector m_interestingPeers first?
    Swarm::getInstance().addTaskAfter(0, [this] {
        for(int i = 0; i < Swarm::getInstance().peers().size() * 0.3; ++i) {
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
    Swarm::getInstance().addTaskAfter(m_packetTime, [this, queriedId] {
        auto recipient = Swarm::getInstance().getPeer(queriedId);
        if(recipient != nullptr) {
            recipient->receivePing(m_node.id());
        }
        else{
            LOG("sendPing Warning: the recipient peer does not exist");
        }
    });
    PeerStatistics::setPingCounter(PeerStatistics::pingCounter() + 1);
}

void Peer::receivePing(const ID & queryingId) {
    Swarm::getInstance().addTaskAfter(m_packetTime, [this, queryingId] {
        auto recipient = Swarm::getInstance().getPeer(queryingId);
        if(recipient != nullptr) {
            recipient->receivePingResponse(m_node.id());
        }
        else{
            LOG("receivePing Warning: the recipient peer does not exist");
        }
    });
}

bool Peer::receivePingResponse(const ID& queriedId) {
    Swarm::getInstance().addTaskAfter(m_packetTime, [queriedId] {
        auto recipient = Swarm::getInstance().getPeer(queriedId);
        if(recipient != nullptr) {
            LOG(queriedId << " is alive!");
        }
        else{
            LOG("receivePing Warning: the recipient peer does not exist");
        }
    });
    return true;
}

void Peer::sendFindNode(const ID& recipientId
                        , const ID& senderId
                        , const ID& queriedId) {
    // sender side
    Swarm::getInstance().addTaskAfter(m_packetTime, [recipientId
                                                , queriedId
                                                , senderId] {
        auto recipient = Swarm::getInstance().getPeer(recipientId);
        if(recipient != nullptr) {
            recipient->receiveFindNode(senderId, queriedId);
        }
        else{
            LOG("sendFindNode Warning: the recipient peer "
                << std::hex << recipientId << " does not exist");
        }
    });
    PeerStatistics::setFindNodeCounter(PeerStatistics::findNodeCounter() + 1);
}

void Peer::receiveFindNode(const ID& senderId
                           , const ID& queriedId) {
    // receiver side
    auto idsFound = m_node.receiveFindNode(senderId, queriedId
                                           , boost::chrono::system_clock::now());
    Swarm::getInstance().addTaskAfter(m_packetTime, [senderId
                                                , queriedId
                                                , idsFound] {
        auto recipient = Swarm::getInstance().getPeer(senderId);
        if(recipient != nullptr) {
            recipient->sendFindNodeResponse(senderId, queriedId, idsFound);
        }
        else{
            LOG("receiveFindNode Warning: the recipient peer does not exist");
        }
    });
}

void Peer::sendFindNodeResponse(const ID& receiverId
                                , const ID& queriedId
                                , std::vector<ID> idsFound) {
    // receiver side
    Swarm::getInstance().addTaskAfter(m_packetTime, [this
                                                , receiverId
                                                , queriedId
                                                , idsFound] {
        auto receiver = Swarm::getInstance().getPeer(receiverId);
        if(receiver != nullptr) {
            receiver->receiveFindNodeResponse(queriedId, idsFound, id());
        }
        else {
            LOG("sendFindNodeResponse Warning: the recipient peer does not exist");
        }
    });
}

void Peer::receiveFindNodeResponse(const ID& queriedId
                                   , std::vector<ID> ids
                                   , const ID& responserId) {
    Swarm::getInstance().addTaskAfter(m_packetTime, [this
                                                , ids
                                                , queriedId
                                                , responserId]{
        if(ids.size() == 1 && ids[0] == queriedId) {
            LOG(queriedId << " found from " << responserId);
        }
        else {
            LOG(queriedId << " not found");
            for(auto& id : ids) {
                sendFindNode(id, m_node.id(),queriedId);
            }
        }
    });
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

