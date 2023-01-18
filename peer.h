#pragma once

#include "node.h"

class Peer
{
private:
    Node         m_node;

public:
    Peer();
    Peer(const Node & node) : m_node(node) {};
};

