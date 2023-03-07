#pragma once

#include "Node.h"
#include "ID.h"
#include "IDhtTransportProtocol.h"
#include "EventQueue.h"
#include "PeerStatistics.h"

inline uint64_t FindLimit = 50;

class Peer: public PeerStatistics, public IDhtTransportProtocol, public ITimer
{
private:
    Node                                m_node;
    EventQueue::Interval                m_packetTime;
    static std::mt19937                 s_randomGenerator;
    //std::vector<std::shared_ptr<Peer>>  m_interestingPeers; // fill within start()

public:
    Peer() = delete;
    Peer(const Peer&) = default;
    Peer(Peer&&) = default;
    Peer(ID id,
         bool useTcp);

    ID id();
    Node& node();
    NodeInfo& info();
    EventQueue::Interval packetTime() const;
    void randomize();
    ID pickRandomPeer();

    void start(const ID & bootstrapId);

    bool addNode(const ID & id);

    virtual void sendPing(const ID & queriedId) override;
    void sendPingResponse(const ID & requestorId, const ID & queriedId);

    void receivePing(const ID & requestorId);
    void receivePingResponse(bool online, const ID & queriedId);

    virtual void sendFindNode(const ID& recipientId
                      , const ID& initiatorId
                      , const ID& queriedId) override;

    void receiveFindNode(const ID& initiatorId
                         , const ID& queriedId);

    void receiveFindNodeResponse(const ID & queriedId
                                 , std::vector<ID> ids
                                 , const ID& responserId);

    void onFindNodeResponse(bool find) override;
    void onPingResponse(bool find) override;
    void onPacketReceived() override;
    void onPacketSent() override;
    void onBootstrap() override;

    virtual void startTimer(EventQueue::Interval duration, std::function<void ()> F) override;


};
