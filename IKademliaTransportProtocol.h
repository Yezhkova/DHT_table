#pragma once

#include "ID.h"
class IKademliaTransportProtocol
{
public:
    virtual ~IKademliaTransportProtocol() = default;
    virtual void ping(const ID & queryingId, const ID & queriedId) = 0;
    virtual void pingResponse(const ID & queryingId, const ID & queriedId) = 0;

    virtual void findNode(const ID & myId, const ID & queriedId) = 0;
    virtual void findNodeResponse(const ID & myId, const ID & queriedId) = 0;


// TODO:   virtual void store(const ID & id) = 0;
// TODO:   virtual void findValue(const ID & myId, const ID & queriedId) = 0;
};
