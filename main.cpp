#include "SwarmSimulator.h"
#include "Utils.h"

int main(void) {

    SwarmSimulator simulator(false, 10);
    simulator.addTask([&](SwarmSimulator & simulatorRef)
    {
        auto Peers = simulatorRef.getSwarm().Peers();
        for(auto & Peer1 : Peers)
        {
            for(auto & Peer2 : Peers)
            {
                Peer2.ping(Peer1); // can print
                // simulatorRef.addTask([&]{Peer2.ping(Peer1);});
            }
        }
    });
    simulator.run();
    LOG("simulation done");
    return 0;
}



