#pragma once

#include "ID.h"
#include "EventQueue.h"
class IDhtTransportProtocol
{
public:
    virtual ~IDhtTransportProtocol() = default;
    virtual void sendPing(const ID & queriedId) = 0;

    virtual void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId) = 0;


// TODO:   virtual void store(const ID & id) = 0;
// TODO:   virtual void findValue(const ID & myId, const ID & queriedId) = 0;
};

class ITimer
{
public:
    virtual void startTimer(EventQueue::Interval delay, std::function<void()>&&) = 0;
};
