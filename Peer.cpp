#include "Peer.h"

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

void Peer::sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId)
{
    swarm()->getPeer(recipientId)->node().receiveFindNode()

}

void receiveFindNode(const ID & recipientId, const ID & senderId, const ID & queriedId)
{

}
