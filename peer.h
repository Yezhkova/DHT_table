#pragma once

#include "node.h"

class Peer
{
private:
    Node         m_node;

public:
    Peer() = default;
    Peer(const Node & node) : m_node(node) {};

    void randomize();
};

