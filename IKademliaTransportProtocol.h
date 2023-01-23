#pragma once

#include "ID.h"
class IKademliaTransportProtocol
{
public:
    virtual ~IKademliaTransportProtocol() = default;
    virtual void ping(const ID & id) = 0;
    virtual void pingResponse(const ID & queryingId, const ID & queriedId) = 0;

    virtual void findNode(ID myId, ID queriedId) = 0;

// TODO:   virtual void store(ID id) = 0;
// TODO:   virtual void findValue(ID myId, ID queriedId) = 0;
};
