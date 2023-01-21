#pragma once

#include "Node.h"

class Peer
{
private:
    Node         m_Node;

public:
    Peer() = default;
    Peer(const Node & node) : m_Node(node) {};

    void randomize();
    void ping(Peer peer);
};

