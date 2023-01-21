#pragma once

#include "ISimulationTask.h"
#include "Swarm.h"

class SwarmSimulator
{
private:
    Swarm                       m_Swarm;
    ISimulationTask             m_simTask;
    boost::asio::io_context     m_ioContext;

public:
    SwarmSimulator(bool useTcpMode, int initialPeerNumber);
    void addTask(std::function<void (SwarmSimulator&)>);
    void addTask(std::function<void ()>);

    void run();
    void stop();

    Swarm& getSwarm() { return m_Swarm; }
};

