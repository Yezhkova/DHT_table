#pragma once

#include "Node.h"
#include "ID.h"
#include "IKademliaTransportProtocol.h"
#include "EventQueue.h"
#include "PeerStatistics.h"

inline uint64_t FindLimit = 50;

class Peer: public PeerStatistics
{
private:
    Node                                m_node;
    EventQueue::Interval                m_packetTime;
    static std::mt19937                 s_randomGenerator;
    static uint64_t                     s_label;
    uint64_t                            m_label;
    //std::vector<std::shared_ptr<Peer>>  m_interestingPeers; // fill within start()

public:
    Peer() = delete;
    Peer(const Peer&) = default;
    Peer(Peer&&) = default;
    Peer(ID id,
         IKademliaTransportProtocol& protocol,
         bool useTcp);

    ID id();
    Node& node();
    NodeInfo& info();
    EventQueue::Interval packetTime() const;
    uint64_t label() const;

    void randomize();
    ID pickRandomPeer();

    void start(const ID & bootstrapId);

    bool addNode(const ID & id);

    void sendPing(const ID & queriedId);
    void sendPingResponse(const ID & queryingId, const ID & queriedId);

    void receivePing(const ID & queryingId);
    bool receivePingResponse(const ID & queriedId);

    void sendFindNode(const ID& recipientId
                      , const ID& initiatorId
                      , const ID& queriedId);

    void receiveFindNode(const ID& initiatorId
                         , const ID& queriedId);

    void receiveFindNodeResponse(const ID & queriedId
                                 , std::vector<ID> ids
                                 , const ID& responserId);

    void onFindNodeResponse(bool find) override;
    void onPacketReceived() override;
    void onPacketSent() override;
    void onBootstrap() override;
};
