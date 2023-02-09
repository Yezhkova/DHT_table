#include "SwarmSimulator.h"
#include "Utils.h"
#include "iostream"

#define TCP true
#define SIM false

int main(void) {
    SwarmSimulator simulator;
    simulator.init(SIM, 5);
    simulator.addTask([&simulator]
    {
        Swarm swarm = Swarm::getInstace();
        auto peers = swarm.peers();

        for(auto & peer1 : peers)
        {
            LOG(peer1.second->id() << " enters the swarm");
            if( peer1.second->id() != swarm.bootstrapNode()->id()){
                peer1.second->start(swarm.bootstrapNode()->id());
            }
        }
    });
    simulator.run();
    LOG("simulation done");
    return 0;
}



