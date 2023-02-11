#pragma once

#include "ID.h"
#include "IKademliaTransportProtocol.h"
#include "Peer.h"
#include <map>
#include <memory>
#include <random>

class Swarm : public IKademliaTransportProtocol
{
private:
    boost::asio::io_context               m_ioContext;
    std::map<ID, std::shared_ptr<Peer>>   m_peers;
    std::shared_ptr<Peer>                 m_bootstrapNode;
    bool                                  m_useTcp = false;

    Swarm() {};

public:

    static Swarm& getInstance();

    void init(bool mode, int PeerNumber);
    void addTask(std::function<void ()> F);
    void run();
    void stop();

    void generateSwarm(bool mode, size_t Peers);
    void createBootstrapNode();
    bool tcp();
    std::shared_ptr<Peer>& bootstrapNode();
    const std::map<ID, std::shared_ptr<Peer>>& peers() const;
    std::shared_ptr<Peer> getPeer(const ID& id);

    virtual void sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId) override;
    virtual void sendPingResponse(const ID & queryingId, const ID & queriedId) override;

    virtual void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId) override;
    virtual void sendFindNodeResponse(const ID & myId, const ID & queriedId) override;

};
