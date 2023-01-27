#pragma once

#include "Node.h"

class Peer
{
private:
    Node m_node;

public:
    Peer() = delete;
    Peer(Peer && ) = default;

    Peer(ID id, IKademliaTransportProtocol& protocol) : m_node(id, protocol) {};

    const ID & id() const { return m_node.id(); };
    Node & node() { return m_node; };
    void randomize();
    void bootstrap(Peer * bootPeer);
    void ping(const Peer & peer);
    void ping(const ID & id);
};

