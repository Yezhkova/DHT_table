#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"

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
    std::cout << m_node.bucketMap();
//    LOG(m_node.bucketMap());
    sendFindNode(bootstrapId, m_node.id(),  m_node.id());
}

void Peer::sendFindNode(const ID & recipientId,
                        const ID & myId, const ID & queriedId)
{
    auto peer = Swarm::getInstace().getPeer(recipientId);
    if(peer != nullptr)
    {
        peer->node().receiveFindNode(recipientId, myId, queriedId);
    }
    else{
        LOG("sendFindNode Warning: the recipient peer does not exist");
    }
}

void Peer::receiveFindNode(const ID & recipientId,
                     const ID & senderId, const ID & queriedId)
{

}

void Peer::receiveFindNodeResponse(const ID & senderId, const ID & queriedId)
{
    LOG("received node " << queriedId <<
        " from node "    << senderId);
    m_node.updateNode(senderId);
}

void Peer::onFindNodeResponse(bool find, int packetNumber)
{

}
void Peer::onPacketReceived()
{

}
void Peer::onPacketSent()
{

}

