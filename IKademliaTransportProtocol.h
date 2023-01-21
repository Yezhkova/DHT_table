#pragma once

#include "ID.h"
class IKademliaTransportProtocol
{
public:
    virtual ~IKademliaTransportProtocol() = default;
    virtual void ping(ID myid) = 0;
    virtual void findNode(ID myId, ID queriedId) = 0;
    virtual void findValue(ID myId, ID queriedId) = 0;
};
