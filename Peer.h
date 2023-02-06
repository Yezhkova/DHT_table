#pragma once

#include "INodeEventHandler.h"
#include "Node.h"
#include "ID.h"
#include "IKademliaTransportProtocol.h"

class Peer: public INodeEventHandler
{
private:
    Node                  m_node;

public:
    Peer() = delete;
    Peer(const Peer&) = default;
    Peer(Peer && ) = default;
    Peer(ID id,
         IKademliaTransportProtocol& protocol,
         bool useTcp)
        : m_node(id, protocol, *this) {};

    ID id();
    Node& node();
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
    void receiveFindNodeResponse(const ID & senderId, const ID & queriedId);

    void onFindNodeResponse(bool find, int packetNumber) override;
    void onPacketReceived() override;
    void onPacketSent() override;
};
