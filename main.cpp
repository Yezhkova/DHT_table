#include "SwarmSimulator.h"
#include "Utils.h"

#define TCP true
#define SIM false

int main(void) {

    SwarmSimulator simulator(SIM, 2);
    simulator.addTask(std::make_shared<SwarmSimulator>(simulator), [&](SwarmSimulator & simulatorRef) // [&] = this?
    {
        auto Peers = simulatorRef.getSwarm().Peers();
        for(auto & Peer1 : Peers)
        {
            for(auto & Peer2 : Peers)
            {
                Peer2.ping(Peer1); // can print
                // simulatorRef.addTask([&] (SwarmSimulator & simulatorRef) {Peer2.ping(Peer1);}); // for TCP simulator
            }
        }
    });
    simulator.run();
    LOG("simulation done");
    return 0;
}



