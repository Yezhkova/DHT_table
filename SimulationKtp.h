#pragma once

#include "ID.h"
//#include "Swarm.h"
#include "Utils.h"
#include <memory>

class Swarm;
class SimulationKtp //: public IKademliaTransportProtocol
{
    //std::shared_ptr<Swarm> m_Swarm;

public:
    SimulationKtp(){};
//    SimulationKtp(std::shared_ptr<Swarm> swarm) : m_Swarm(swarm) {}; // will give ~SimulationKtp() error if uncommented

    void ping(const ID & id);
    void findNode(ID myId, ID queriedId){}
    void findValue(ID myId, ID queriedId) {}

};

