#pragma once


#include "INodeEventHandler.h"
#include <cstdint>
class PeerStatistics: public INodeEventHandler
{
    uint64_t m_packetCounter = 0;
    uint64_t m_findNodeCounter = 0;
    uint64_t m_failedFindNodeCounter = 0; // timeout or packet limit/threshold

public:
    PeerStatistics();
};

