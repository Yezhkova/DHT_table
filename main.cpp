#include "SwarmSimulator.h"
#include "Utils.h"

#define TCP true
#define SIM false

int main(void) {

//    auto simulator = std::make_shared<SwarmSimulator>(SIM, 2);

    SwarmSimulator simulator(SIM,2);
    simulator.addTask([simulator]
    {
        auto Peers = simulator.getSwarm().peers();
        for(auto & Peer1 : Peers)
        {
            for(auto & Peer2 : Peers)
            {
                Peer2.second->ping(*Peer1.second); // can print
                // simulatorRef.addTask([&] (SwarmSimulator & simulatorRef) {Peer2.ping(Peer1);}); // for TCP simulator
            }
        }
    });
    simulator.run();
    LOG("simulation done");
    return 0;
}



