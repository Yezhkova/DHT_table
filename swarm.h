#pragma once

#include "peer.h"

class Swarm
{
private:
    std::vector<Peer> m_peers;
    Peer              m_bootstrapNode;
    bool              m_useTcp;
public:
    Swarm(bool mode, int peerNumber) : m_useTcp(mode) {generateSwarm(peerNumber);};

    void generateSwarm(size_t peers);
    void createBootstrapNode();
};

// tcp & offline
