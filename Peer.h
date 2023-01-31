#pragma once

#include "Node.h"
#include "Swarm.h"
//class Swarm;

class Peer
{
private:
    Node                    m_node;
    std::shared_ptr<Swarm>  m_swarm = nullptr;

public:
    Peer() = delete;
    Peer(Peer && ) = default;
    Peer(ID id,
         IKademliaTransportProtocol& protocol,
         std::shared_ptr<Swarm> swarm)
        : m_node(id, protocol)
        , m_swarm(swarm) {};

    ID id() { return m_node.id(); };
    Node & node() { return m_node; };
    std::shared_ptr<Swarm> swarm() { return m_swarm; } ;
    void randomize();

    void start(const ID & bootstrapId);

    void addNode(const ID & id);

    void sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId);
    void sendPingResponse(const ID & queryingId, const ID & queriedId);

    void receivePing(const ID & queryingId, const ID & queriedId);
    void receivePingResponse(const ID & queryingId, const ID & queriedId);

    void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId);    
    void sendFindNodeResponse(const ID & myId, const ID & queriedId);

    void receiveFindNode(const ID & recipientId, const ID & myId, const ID & queriedId);
    void receiveFindNodeResponse(const ID & recipientId, const ID & myId, const ID & queriedId);

};

