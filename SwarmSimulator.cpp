#include "SwarmSimulator.h"

SwarmSimulator::SwarmSimulator(bool useTcpMode, int initialPeerNumber)
    : m_Swarm(useTcpMode, initialPeerNumber) {}

SwarmSimulator::SwarmSimulator(const SwarmSimulator& ss)
    : m_Swarm(ss.m_Swarm)
    , m_simTask(ss.m_simTask) {}

const Swarm& SwarmSimulator::getSwarm() const
{
    return m_Swarm;
}

void SwarmSimulator::addTask(std::function<void ()> F)
{
    m_ioContext.post(F);
}

void SwarmSimulator::run()
{
    m_ioContext.run();
}

void SwarmSimulator::stop()
{
    m_ioContext.stop();
}

