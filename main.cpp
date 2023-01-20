#include "ID.h"
#include "utils.h"
#include "swarmsimulator.h"

int main(void) {

    SwarmSimulator simulator(false); // как у Swarm
    simulator.addTask([&](SwarmSimulator & simulatorRef){
        auto & peers = simulatorRef.swarm().peers();
        for(auto & peer1 : peers)
        {
            for(auto & peer2 : peers)
            {
                peer2.ping(peer1); // can print
                // simulatorRef.addTask([&]{peer2.ping(peer1);});
            }
        }
    });
    simulator.run();
    LOG("simulation done");
    return 0;
}



