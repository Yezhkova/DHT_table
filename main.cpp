#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false

void calculateStatistic(const Swarm& swarm) {
    // for auto peer: peers
    // calculate statistics
    // djkfgnaho
}

int main(void) {
    Swarm& swarm = Swarm::getInstance();
    swarm.init(SIM, 20);
    swarm.addTask(0, [&swarm]
    {
        auto peers = swarm.peers();
        for(auto & peer1 : peers)
        {
            LOG(peer1.second->id() << " enters the swarm");
            if( peer1.second->id() != swarm.bootstrapNode()->id()){
                peer1.second->start(swarm.bootstrapNode()->id());
            }
        }
    });
    swarm.addTask(10*60*1000, [&swarm]
    {
        calculateStatistic(swarm);
    });
    swarm.run();
    LOG("simulation done");
    return 0;
}



