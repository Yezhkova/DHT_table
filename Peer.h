#pragma once

#include "Node.h"
#include "SimulationKtp.h"
#include "TcpKtp.h"

class Peer
{
private:
    Node                            m_Node;
//    IKademliaTransportProtocol&     m_protocol;

public:
    Peer() = delete;
    Peer(Peer && ) = default;

    Peer(ID id, IKademliaTransportProtocol& protocol) : m_Node(id, protocol) {};

    const ID & id() const { return m_Node.getNodeId(); };
    void randomize();
    void ping(const Peer & peer);
    void ping(const ID & id)
    {
        m_Node.ping(id);
    };
};

