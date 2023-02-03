#pragma once

#include "ISimulationTask.h"
#include "Swarm.h"
#include <queue>
#include <utility>

class SwarmSimulator //: public std::enable_shared_from_this<SwarmSimulator>
{
private:
//    ISimulationTask           m_simTask;
    boost::asio::io_context   m_ioContext;

public:

    SwarmSimulator() = default;
    void init(bool useTcpMode, int initialPeerNumber);

    void addTask(std::function<void ()> F);

    void run();
    void stop();

    const Swarm& getSwarm() const;
};

