#include "SwarmSimulator.h"
#include "Utils.h"

#define TCP true
#define SIM false

int main(void) {

    auto simulator = std::make_shared<SwarmSimulator>(SIM, 2);
    simulator->addTask([&simulator]
    {
        Swarm swarm = simulator->getSwarm();
        auto peers = swarm.peers();
        for(auto & peer1 : peers)
        {
            for(auto & peer2 : peers)
            {
                swarm.ping(peer1.second->id(), peer2.second->id());
//                Peer2.second->ping(*Peer1.second);
                // simulatorRef.addTask([&] (SwarmSimulator & simulatorRef) {Peer2.ping(Peer1);}); // for TCP simulator
            }
        }
    });
    simulator->run();
    LOG("simulation done");
    return 0;
}



