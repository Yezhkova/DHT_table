#include "Swarm.h"

void Swarm::generateSwarm(size_t Peers, bool mode)
{
    for(size_t i = 0; i < Peers; ++i)
    {
        ID id; // randomize
        id.randomize();
        m_peers[id] = std::make_shared<Peer>(id, *this);
    }
}

