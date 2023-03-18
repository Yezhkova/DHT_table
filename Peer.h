#pragma once

#include "Node.h"
#include "ID.h"
#include "IDhtTransportProtocol.h"
#include "EventQueue.h"
#include "PeerStatistics.h"

class Peer: public PeerStatistics, public IDhtTransportProtocol, public ITimer
{
private:
    Node                                m_node;
    EventQueue::Interval                m_packetTime;
    static std::mt19937                 s_randomGenerator;
public:
    Peer() = delete;
    Peer(const Peer&) = default;
    Peer(Peer&&) = default;
    Peer(ID id, bool useTcp);

    const ID& id() const;
    Node& node();
    EventQueue::Interval packetTime() const;
    void randomize();
    ID pickRandomPeer();

    void start(const ID & bootstrapId);

    virtual void sendPing(const ID & queriedId) override;

    void receivePing(const ID & requestorId);
    void receivePingResponse(bool online, const ID & queriedId);

    virtual void sendFindNode(const ID& recipientId
                      , const ID& initiatorId
                      , const ID& queriedId) override;

    void receiveFindNode(const ID& initiatorId
                         , const ID& queriedId);

    void receiveFindNodeResponse(const ID & queriedId
                                 , const std::vector<const ID*>& ids
                                 , const ID& responserId);

    void onFindNodeResponse(bool find) override;
    void onPingResponse(bool find) override;
    void onPacketReceived() override;
    void onPacketSent() override;
    void findRandomNodes(int nodeNumber);
    void onNodeNotFound() override;
    void onNodeFound() override;
    virtual void startTimer(EventQueue::Interval duration, std::function<void ()>&& F) override;

};
