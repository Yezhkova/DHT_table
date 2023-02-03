#include "SwarmSimulator.h"

void SwarmSimulator::init(bool useTcpMode, int initialPeerNumber)
{
    Swarm::getInstace().init(useTcpMode, initialPeerNumber);
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

