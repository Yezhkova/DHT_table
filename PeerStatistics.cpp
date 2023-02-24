#include "PeerStatistics.h"
#include <cstdint>

uint64_t PeerStatistics::findNode() const
{
    return m_findNodeCounter;
}

//void PeerStatistics::setFindNodeCounter(uint64_t newFindNodeCounter)
//{
//    m_findNodeCounter = newFindNodeCounter;
//}

uint64_t PeerStatistics::ping() const {
    return m_pingCounter;
}

void PeerStatistics::setPingCounter(uint64_t newPingCounter) {
    m_pingCounter = newPingCounter;
}
