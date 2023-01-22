#pragma once

#include "ISimulationTask.h"
#include "Swarm.h"
#include <queue>
#include <utility>

class SwarmSimulator : public std::enable_shared_from_this<SwarmSimulator>
{
private:
    Swarm                                                   m_Swarm;
    ISimulationTask                                         m_simTask;
    boost::asio::io_context                                 m_ioContext;
    std::queue<std::pair
        <std::shared_ptr<SwarmSimulator>,
        std::function<void (SwarmSimulator&)>>>             m_tasks;

public:

    SwarmSimulator(bool useTcpMode, int initialPeerNumber);
    SwarmSimulator(const SwarmSimulator& ss);

    void addTask(std::shared_ptr<SwarmSimulator>(simulator), std::function<void (SwarmSimulator&)> F);

    void run();
    void stop();

    Swarm& getSwarm() { return m_Swarm; }
};

