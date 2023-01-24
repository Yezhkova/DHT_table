#include "Peer.h"

void Peer::randomize()
{
    m_Node.randomizeId();
}

void Peer::ping(const Peer & otherPeer)
{
    m_Node.protocol().ping(otherPeer.m_Node.getNodeId());
}

