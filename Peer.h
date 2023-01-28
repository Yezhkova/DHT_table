#pragma once

#include "Node.h"

class Peer
{
private:
    Node m_node;

public:
    Peer() = delete;
    Peer(Peer && ) = default;
    Peer(ID id, IKademliaTransportProtocol& protocol) : m_node(id, protocol) {};

    const ID & id() const { return m_node.id(); };
    Node & node() { return m_node; };
    void randomize();

    void start(const ID & bootstrapId);

    void addNode(const ID & id);

    void sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId);
    void sendPingResponse(const ID & queryingId, const ID & queriedId);
    void receivePing(const ID & queryingId, const ID & queriedId);
    void receivePingResponse(const ID & queryingId, const ID & queriedId);

    void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId);
    void sendFindNodeResponse(const ID & myId, const ID & queriedId);
    void receiveFindNode(const ID & myId, const ID & queriedId);
    void receiveFindNodeResponse(const ID & myId, const ID & queriedId);

};

