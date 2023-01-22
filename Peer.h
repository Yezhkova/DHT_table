#pragma once

#include "Node.h"

class Peer
{
private:
    Node         m_Node;
    bool         m_useTcp;

public:
    Peer() = default;
    Peer(const Node & node) : m_Node(node) {};

    void randomize();
    void setTcp(bool mode) { m_useTcp = mode; };
    void ping(Peer peer);
};

