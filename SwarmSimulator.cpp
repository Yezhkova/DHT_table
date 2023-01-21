#include "SwarmSimulator.h"

SwarmSimulator::SwarmSimulator(bool useTcpMode, int initialPeerNumber)
    : m_Swarm(useTcpMode, initialPeerNumber) {};

void SwarmSimulator::run()
{

}

void SwarmSimulator::addTask(std::function<void (SwarmSimulator&)>)
{

}
