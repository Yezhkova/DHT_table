#include "Swarm.h"

void Swarm::generateSwarm(size_t Peers, bool mode)
{
    for(size_t i = 0; i < Peers; ++i)
    {
        ID id;
        id.randomize();
        m_peers[id] = std::make_shared<Peer>(id, *this, shared_from_this());
    }
}

void Swarm::sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId)
{
    if(auto it = m_peers.find(queryingId); it != m_peers.end())
    {
//        it->second->node().ping(queriedId);
        //TODO: wait for the answer for some time
        //      receive message "i'm alive"
        //      if online: LOG(std::string(queriedId) << " is online\n");
        //      else LOG(std::string(queriedId) << " failed to respond\n")
    }
}

void Swarm::sendPingResponse(const ID & queryingId, const ID & queriedId)
{

}

void Swarm::sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId)
{

}

void Swarm::sendFindNodeResponse(const ID & myId, const ID & queriedId)
{

}


