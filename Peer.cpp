#include "Swarm.h"
#include "Utils.h"

ID Peer::id() {
    return m_node.id();
}

Node& Peer::node() {
    return m_node;
}

std::weak_ptr<Swarm> Peer::swarm() {
    return m_swarm;
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
    if (std::shared_ptr<Swarm> spt = m_swarm.lock()) {
        spt->getPeer(recipientId)->node()
            .receiveFindNode(recipientId,myId, queriedId);
    }
    else {
        LOG("sendFindNode error: cannot lock weak ptr");
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

