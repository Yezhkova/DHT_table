#include "Peer.h"

void Peer::randomize()
{
    m_Node.randomizeId();
}

void Peer::ping(Peer peer)
{
    m_Node.ping(peer.m_Node.getNodeId());
}
