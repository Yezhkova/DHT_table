#pragma once

#include "Node.h"
#include "SimulationKtp.h"
#include "TcpKtp.h"

class Peer
{
private:
    Node                                            m_Node;
    std::shared_ptr<IKademliaTransportProtocol>     m_protocol = nullptr;

public:
    Peer() = default;
    Peer(bool useTcp)
    {
        if (!useTcp)
        {
            m_protocol = std::make_shared<SimulationKtp>();
        }
        else {
            m_protocol = std::make_shared<TcpKtp>();
        }
    };

    Peer(const Node & node) : m_Node(node) {};

    void randomize();
    void ping(Peer peer);
};

