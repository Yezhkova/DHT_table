#pragma once

#include "isimulationtask.h"
#include "swarm.h"

class SwarmSimulator
{
private:
    Swarm                       m_swarm;
    ISimulationTask             m_sim_task;
    boost::asio::io_context     m_io_context;

public:
    SwarmSimulator(bool useTcpMode, int initialPeerNumber);
    void addTask(std::function<void (SwarmSimulator&)>);
    void addTask(std::function<void ()>);

    void run();
    void stop();

    Swarm& swarm() { return m_swarm; }
};

