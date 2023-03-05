#pragma once

#include "ID.h"
#include "IKademliaTransportProtocol.h"
#include "Peer.h"
#include <map>
#include <memory>
#include <random>
#include "EventQueue.h"

class Swarm : public IKademliaTransportProtocol, public ITimer
{
private:
//    boost::asio::io_context               m_ioContext;
    EventQueue                            m_eventQueqe;
    std::map<ID, std::shared_ptr<Peer>>   m_peers;
    std::shared_ptr<Peer>                 m_bootstrapNode;
    bool                                  m_useTcp = false;

    Swarm() {};

public:

    static Swarm& getInstance();

    void init(bool mode, int PeerNumber);
    void addTaskAfter(EventQueue::Interval duration, std::function<void ()> F);
    virtual void startTimer(EventQueue::Interval duration, std::function<void ()> F) override
    {
        addTaskAfter(duration, F);
    }

    void run();
    void stop();

    void generateSwarm(bool mode, size_t Peers);
    bool tcp();
    std::shared_ptr<Peer>& bootstrapNode();
    const std::map<ID, std::shared_ptr<Peer>>& peers() const;
    std::shared_ptr<Peer> getPeer(const ID& id);
    EventQueue& eventQueqe();

private:

    virtual void sendPingInSwarm(const ID & requestorId
                                 , const ID & queriedId) override;
    virtual void sendFindNodeInSwarm(const ID & recipientId
                                     , const ID & initiatorId
                                     , const ID & queriedId) override;
    // startTimer
};
