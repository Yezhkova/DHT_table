#include "Peer.h"

void Peer::randomize()
{
    m_node.randomizeId();
}

void Peer::bootstrap(Peer* bootPeer)
{
    m_node.addNode(&bootPeer->node());
    m_node.populate(&bootPeer->node());//.id());
}

void Peer::ping(const Peer & otherPeer)
{
//    m_node.protocol().ping(otherPeer.m_node.id());
}

void Peer::ping(const ID & id)
{
    m_node.ping(id);
}
