#include "Swarm.h"

void Swarm::generateSwarm(size_t Peers)
{
    for(size_t i = 0; i < Peers; ++i)
    {
        Peer p;
        p.randomize();
        m_Peers.push_back(p);
    }
}
