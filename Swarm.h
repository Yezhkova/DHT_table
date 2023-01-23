#pragma once

#include "Peer.h"
#include "SimulationKtp.h"

class Swarm : public IKademliaTransportProtocol, public std::enable_shared_from_this<Swarm>
{
private:
    std::map<ID, std::shared_ptr<Peer>>             m_peers;
    Peer*                                           m_bootstrapNode = nullptr;
    bool                                            m_useTcp;

public:
    Swarm(bool mode, int PeerNumber)
        : m_useTcp(mode)
    {
        generateSwarm(PeerNumber, mode);
    };

    void generateSwarm(size_t Peers, bool mode);
    void createBootstrapNode();
    bool tcp() { return m_useTcp; };
    std::map<ID, std::shared_ptr<Peer>> Peers() { return m_peers; };

    void ping(const ID & id) override
    {
        if(auto it = m_peers.find(id); it != m_peers.end())
        {
            it->second->ping(id);
        }
    };
    void store(ID id) override {};
    void findNode(ID myId, ID queriedId) override {};
    void findValue(ID myId, ID queriedId) override {};
};

// tcp & offline
