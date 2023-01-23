#pragma once

#include "Peer.h"
#include "SimulationKtp.h"

class Swarm : public std::enable_shared_from_this<Swarm>
{
private:
    std::vector<Peer>                               m_peers;
    Peer                                            m_bootstrapNode;
    bool                                            m_useTcp;

public:
    Swarm(const Swarm & swarm)
        : m_peers(swarm.m_peers)
        , m_bootstrapNode(swarm.m_bootstrapNode)
        , m_useTcp(swarm.m_useTcp) {};

    Swarm(bool mode, int PeerNumber)
        : m_useTcp(mode)
    {
        generateSwarm(PeerNumber, mode);
    };

    void generateSwarm(size_t Peers, bool mode);
    void createBootstrapNode();
    bool tcp() { return m_useTcp; };
    std::vector<Peer> Peers() { return m_peers; };
};

// tcp & offline
