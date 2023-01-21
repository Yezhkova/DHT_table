#pragma once


#include "ID.h"
#include "Swarm.h"
class SimulationKtp
{
    std::shared_ptr<Swarm> m_Swarm;
public:
    SimulationKtp(std::shared_ptr<Swarm>);
    void ping(ID id){}
    void findNode(ID myId, ID queriedId){}
    void findValue(ID myId, ID queriedId) {}

};

