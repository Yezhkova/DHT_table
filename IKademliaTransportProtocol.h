#pragma once

#include "ID.h"
class IKademliaTransportProtocol
{
public:
    virtual ~IKademliaTransportProtocol() = default;
    virtual void sendPingInSwarm(const ID & requestorId
                                 , const ID & queriedId) = 0;
    virtual void sendFindNodeInSwarm(const ID & recipientId, const ID & myId, const ID & queriedId) = 0;


// TODO:   virtual void store(const ID & id) = 0;
// TODO:   virtual void findValue(const ID & myId, const ID & queriedId) = 0;
};

