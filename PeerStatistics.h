#pragma once

#include "INodeEventHandler.h"
#include <cstdint>
class PeerStatistics: public INodeEventHandler
{
    uint64_t m_packetCounter = 0;
    uint64_t m_pingCounter = 0;
    uint64_t m_findNodeCounter = 0;
    uint64_t m_receiveFindNodeCounter = 0;
    bool     m_failedFindNode = false; // timeout or packet limit/threshold

public:
    PeerStatistics() {};

    uint64_t packets() const { return m_packetCounter; }
    void incPacketCounter() { ++m_packetCounter; }

    uint64_t ping() const { return m_pingCounter; }
    void incPingCounter() { ++m_pingCounter; }

    uint64_t findNode() const { return m_findNodeCounter; }
    void incFindNodeCounter() { ++m_findNodeCounter; }

    uint64_t receiveFindNode() const { return m_receiveFindNodeCounter; }
    void incReceiveFindNodeCounter() { ++m_receiveFindNodeCounter; }

    bool failedFindNode() const { return m_failedFindNode; }
    void setFailedFindNode() { m_failedFindNode = true; }

};


