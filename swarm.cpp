#include "swarm.h"

Swarm::Swarm(bool mode) {}

void Swarm::generate_swarm(size_t peers)
{
    for(size_t i = 0; i < peers; ++i)
    {
        Peer p;
        p.randomize();
        m_peers.push_back(p);
    }
}
