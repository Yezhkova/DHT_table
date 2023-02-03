#include "Swarm.h"
#include "Utils.h"

/*Peer::Peer(const Peer& p)
    : m_swarm(std::move(p.m_swarm))
    , m_node(std::move(p.m_node)) { }*/

ID Peer::id() {
    return m_node.id();
}

Node& Peer::node() {
    return m_node;
}

void Peer::randomize()
{
    m_node.randomizeId();
}

void Peer::addNode(const ID & id)
{
    m_node.addNode(id);
}

void Peer::start(const ID & bootstrapId)
{
    addNode(bootstrapId);
    sendFindNode(bootstrapId, m_node.id(),  m_node.id());
}

void Peer::sendFindNode(const ID & recipientId,
                        const ID & myId, const ID & queriedId)
{
    if (std::shared_ptr<Swarm> sptSwarm = m_swarm.lock()) {
        sptSwarm->getPeer(recipientId)->node()
            .receiveFindNode(recipientId,myId, queriedId);
    }
    else {
        LOG("sendFindNode error: cannot lock Swarm weak_ptr");
    }
}

void Peer::receiveFindNode(const ID & recipientId,
                     const ID & senderId, const ID & queriedId)
{

}

void Peer::receiveFindNodeResponse(const ID & senderId, const ID & queriedId)
{
    LOG("received node " << std::string(queriedId) <<
        " from node "    << std::string(senderId));
    m_node.updateNode(senderId);
}

