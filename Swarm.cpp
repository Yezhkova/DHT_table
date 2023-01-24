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

void Swarm::ping(const ID & id)
{
    if(auto it = m_peers.find(id); it != m_peers.end())
    {
        it->second->ping(id);
    }
}

void Swarm::pingResponse(const ID & queryingId, const ID & queriedId)
{

}

void Swarm::findNode(const ID & myId, const ID & queriedId)
{

}

void Swarm::findNodeResponse(const ID & myId, const ID & queriedId)
{

}


