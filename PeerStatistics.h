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

    uint64_t packetsCnt() const { return m_packetCounter; };
    void incPacketCounter() { ++m_packetCounter; }
    void resetPacketCounter() { m_packetCounter = 0; }

    uint64_t pingCnt() const { return m_pingCounter; }
    void incPingCounter() { ++m_pingCounter; }
    void resetPingCounter() { m_pingCounter = 0; }

    uint64_t findNode() const { return m_findNodeCounter; }
    void incFindNodeCounter() { ++m_findNodeCounter; }
    void resetFindNodeCounter() { m_findNodeCounter = 0; }

    uint64_t receiveFindNode() const { return m_receiveFindNodeCounter; }
    void incReceiveFindNodeCounter() { ++m_receiveFindNodeCounter; }
    void resetReceiveFindNodeCounter() { m_receiveFindNodeCounter = 0; }

    bool failedFindNode() const { return m_failedFindNode; }
    void setFailedFindNode() { m_failedFindNode = true; }
    void resetFailedNode() { m_failedFindNode = false; }

};


