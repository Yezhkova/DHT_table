#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false

int main(void) {
    Swarm& swarm = Swarm::getInstance();
    swarm.init(SIM, 3);
    swarm.addTask([&swarm]
    {
        auto peers = swarm.peers();
        for(auto & peer1 : peers)
        {
            LOG(peer1.second->id() << " enters the hall up high");
            if( peer1.second->id() != swarm.bootstrapNode()->id()){
                peer1.second->start(swarm.bootstrapNode()->id());
            }
        }
    });
    swarm.run();
    LOG("simulation done");
    return 0;
}



