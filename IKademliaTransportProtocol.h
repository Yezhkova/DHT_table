#pragma once

#include "ID.h"
class IKademliaTransportProtocol
{
public:
    virtual ~IKademliaTransportProtocol() = default;
    virtual void sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId) = 0;
    virtual void sendPingResponse(const ID & queryingId, const ID & queriedId) = 0;

    virtual void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId) = 0;
    virtual void sendFindNodeResponse(const ID & myId, const ID & queriedId) = 0;


// TODO:   virtual void store(const ID & id) = 0;
// TODO:   virtual void findValue(const ID & myId, const ID & queriedId) = 0;
};
