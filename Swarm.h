#pragma once

#include "Peer.h"

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
        Peer bootstrapNode(ID(), *this);
        m_bootstrapNode = & bootstrapNode;
        generateSwarm(PeerNumber, mode);
    };

    void generateSwarm(size_t Peers, bool mode);
    void createBootstrapNode();
    bool tcp() { return m_useTcp; };
    std::map<ID, std::shared_ptr<Peer>> peers() const { return m_peers; };

    virtual void ping(const ID & queryingId, const ID & queriedId) override;
    virtual void pingResponse(const ID & queryingId, const ID & queriedId) override;

    virtual void findNode(const ID & myId, const ID & queriedId) override;
    virtual void findNodeResponse(const ID & myId, const ID & queriedId) override;

};
