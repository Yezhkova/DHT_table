#pragma once

#include "Peer.h"

class Swarm
{
private:
    std::vector<Peer> m_Peers;
    Peer              m_bootstrapNode;
    bool              m_useTcp;
public:
    Swarm(bool mode, int PeerNumber) : m_useTcp(mode) { generateSwarm(PeerNumber); };

    void generateSwarm(size_t Peers);
    void createBootstrapNode();
    std::vector<Peer> Peers() { return m_Peers; };
};

// tcp & offline
