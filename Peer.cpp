#include "Peer.h"
#include "SimulationKtp.h"

void Peer::randomize()
{
    m_Node.randomizeId();
}

void Peer::ping(Peer otherPeer)
{
    m_protocol->ping(otherPeer.m_Node.getNodeId());
}

