#pragma once

#include "ISimulationTask.h"
#include "Swarm.h"
#include <queue>
#include <utility>

class SwarmSimulator //: public std::enable_shared_from_this<SwarmSimulator>
{
private:
    Swarm                     m_Swarm;
    ISimulationTask           m_simTask;
    boost::asio::io_context   m_ioContext;

public:

    SwarmSimulator(bool useTcpMode, int initialPeerNumber);
    SwarmSimulator(const SwarmSimulator& ss); // copy constructor for shared_ptr < SwarmSimulator >

    void addTask(std::function<void ()> F);

    void run();
    void stop();

    const Swarm& getSwarm() const;
};

