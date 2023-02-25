#pragma once

#include "INodeEventHandler.h"
#include <cstdint>
class PeerStatistics: public INodeEventHandler
{
    uint64_t m_packetCounter = 0;
    uint64_t m_pingCounter = 0;
    uint64_t m_findNodeCounter = 0;
    uint64_t m_failedFindNodeCounter = 0; // timeout or packet limit/threshold

public:
    PeerStatistics() {};

    uint64_t packet() const { return m_packetCounter; }
    void incPacketCounter() { ++m_packetCounter; }

    uint64_t ping() const { return m_pingCounter; }
    void incPingCounter() { ++m_pingCounter; }

    uint64_t findNode() const { return m_findNodeCounter; }
    void incFindNodeCounter() { ++m_findNodeCounter; }

    uint64_t failedFindNode() const { return m_findNodeCounter; }
    void incFailedFindNodeCounter() { ++m_failedFindNodeCounter; }

};


