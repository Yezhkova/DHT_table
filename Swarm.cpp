#include "Swarm.h"

void Swarm::generateSwarm(size_t Peers, bool mode)
{
    for(size_t i = 0; i < Peers; ++i)
    {
        Peer p;
        p.setTcp(mode);
        p.randomize();
        m_Peers.push_back(p);
    }
}
