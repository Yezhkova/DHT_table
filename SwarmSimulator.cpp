#include "SwarmSimulator.h"

SwarmSimulator::SwarmSimulator(bool useTcpMode, int initialPeerNumber)
    : m_Swarm(useTcpMode, initialPeerNumber) {}

SwarmSimulator::SwarmSimulator(const SwarmSimulator& ss)
    : m_Swarm(ss.m_Swarm)
    , m_simTask(ss.m_simTask)
    , m_tasks(ss.m_tasks) {}


void SwarmSimulator::addTask(std::shared_ptr<SwarmSimulator>(simulator), std::function<void (SwarmSimulator&)> F)
{
    m_tasks.push(std::make_pair(simulator, F));
}

void SwarmSimulator::run()
{
    while(!m_tasks.empty())
    {
        auto pair = m_tasks.front();
        std::shared_ptr<SwarmSimulator> arg = pair.first;
        std::function<void (SwarmSimulator&)> function = pair.second;
        function(*arg);
        m_tasks.pop();
    }
}


