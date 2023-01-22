#include "Peer.h"
#include "SimulationKtp.h"

void Peer::randomize()
{
    m_Node.randomizeId();
}

void Peer::ping(Peer otherPeer)
{
//    m_Node.ping(otherPeer.m_Node.getNodeId());
    if(!m_useTcp)
    {
        SimulationKtp protocol;
        protocol.ping(otherPeer.m_Node.getNodeId());
    }

}

