#pragma once

#include "INodeEventHandler.h"
#include <cstdint>
class PeerStatistics: public INodeEventHandler
{
    uint64_t m_packetCounter = 0;
    uint64_t m_pingCounter = 0;
    uint64_t m_findNodeCounter = 0;
    uint64_t m_receiveFindNodeCounter = 0;
    uint64_t m_failedFindNodeCounter = 0; // timeout or packet limit/threshold

    bool m_done = false;

public:
    PeerStatistics() {};

    uint64_t packetsCounter() const { return m_packetCounter; };
    void incPacketCounter() { ++m_packetCounter; }
    void resetPacketCounter() { m_packetCounter = 0; }

    uint64_t pingCounter() const { return m_pingCounter; }
    void incPingCounter() { ++m_pingCounter; }
    void resetPingCounter() { m_pingCounter = 0; }

    uint64_t findNodeCounter() const { return m_findNodeCounter; }
    void incFindNodeCounter() { ++m_findNodeCounter; }
    void resetFindNodeCounter() { m_findNodeCounter = 0; }

    uint64_t receiveFindNodeCounter() const { return m_receiveFindNodeCounter; }
    void incReceiveFindNodeCounter() { ++m_receiveFindNodeCounter; }
    void resetReceiveFindNodeCounter() { m_receiveFindNodeCounter = 0; }

    uint64_t failedFindNodeCounter() const { return m_failedFindNodeCounter; }
    void incFailedFindNode() { ++m_failedFindNodeCounter; }
    void resetFailedNode() { m_failedFindNodeCounter = 0; }

    bool done() const { return m_done; }
    void setDone() { m_done = true; }
    void resetDone() { m_done = false; }

    void resetAllCounters() {
        resetFindNodeCounter();
        resetPacketCounter();
        resetReceiveFindNodeCounter();
        resetFailedNode();
        resetDone();
    }

};


