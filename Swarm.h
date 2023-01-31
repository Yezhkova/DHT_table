#pragma once

#include "ID.h"
#include "IKademliaTransportProtocol.h"
#include "Peer.h"
#include <map>
#include <memory>
class Peer;

class Swarm : public IKademliaTransportProtocol, public std::enable_shared_from_this<Swarm>
{
private:
    std::map<ID, std::shared_ptr<Peer>>  m_peers;
    Peer*                                m_bootstrapNode = nullptr;
    bool                                 m_useTcp;

public:
    Swarm(bool mode, int PeerNumber)
        : m_useTcp(mode)
    {
        Peer bootstrapNode(ID(), *this, shared_from_this());
        m_bootstrapNode = & bootstrapNode;
        generateSwarm(PeerNumber, mode);
    };

    void generateSwarm(size_t Peers, bool mode);
    void createBootstrapNode();
    bool tcp() { return m_useTcp; };
    Peer* bootstrapNode() { return m_bootstrapNode;};
    std::map<ID, std::shared_ptr<Peer>> peers() const { return m_peers; };
    std::shared_ptr<Peer> getPeer(const ID& id) {
        return m_peers.find(id)->second;
    }

    virtual void sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId) override;
    virtual void sendPingResponse(const ID & queryingId, const ID & queriedId) override;

    virtual void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId) override;
    virtual void sendFindNodeResponse(const ID & myId, const ID & queriedId) override;

};
