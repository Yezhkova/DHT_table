#include "Swarm.h"

void Swarm::generateSwarm(size_t Peers, bool mode)
{
    for(size_t i = 0; i < Peers; ++i)
    {
        Peer p(m_useTcp);
        p.randomize();
        m_peers.push_back(p);
    }
}

