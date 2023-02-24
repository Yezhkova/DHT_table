#include "NodeInfo.h"

void NodeInfo::updateLastSeen(boost::chrono::system_clock::time_point time)
{
    m_lastSeen = time;
}

