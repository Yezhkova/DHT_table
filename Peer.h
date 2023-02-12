#pragma once

#include "INodeEventHandler.h"
#include "Node.h"
#include "ID.h"
#include "IKademliaTransportProtocol.h"
#include "EventQueue.h"

class Peer: public INodeEventHandler
{
private:
    Node                                m_node;
    EventQueue::Duration                m_packetTime;
//    std::vector<std::shared_ptr<Peer>>  m_interestingPeers; // fill within start()

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
    NodeInfo& info();
    void randomize();

    void start(const ID & bootstrapId);

    bool addNode(const ID & id);

    void sendPing(const ID & queriedId);
    void sendPingResponse(const ID & queryingId, const ID & queriedId);

    void receivePing(const ID & queryingId, const ID & queriedId);
    bool receivePingResponse(const ID & queriedId);

    void sendFindNode(const ID& recipientId
                      , const ID& senderId
                      , const ID& queriedId);

    void sendFindNodeResponse(const ID& senderId
                              , const ID& queriedId
                              , std::vector<ID> ids);

    void receiveFindNode(const ID& senderId
                         , const ID& queriedId);

    void receiveFindNodeResponse(const ID & queriedId
                                 , std::vector<ID> ids
                                 , const ID& responserId);

    void onFindNodeResponse(bool find, int packetNumber) override;
    void onPacketReceived() override;
    void onPacketSent() override;
};
