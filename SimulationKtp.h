#pragma once

#include "ID.h"
//#include "Swarm.h"
#include "IKademliaTransportProtocol.h"
#include "Utils.h"
#include <memory>

//class Swarm;
class SimulationKtp : public IKademliaTransportProtocol
{
private:
//    std::shared_ptr<Swarm> m_Swarm = nullptr;

public:
    SimulationKtp(){};
//    SimulationKtp(std::shared_ptr<Swarm> swarm) { m_Swarm = std::move(swarm); }; // will give ~SimulationKtp() error if uncommented
    void ping(const ID & id) override;
    void store(ID id) override {};
    void findNode(ID myId, ID queriedId) override {}
    void findValue(ID myId, ID queriedId) override {}

};
