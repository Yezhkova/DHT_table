#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false

void calculateStatistic(const Swarm& swarm) {
    LOG("------------------------------------calculateStatistic-------------------------");
    Swarm& s = Swarm::getInstance();
    auto peers = s.peers();
    for(auto& p : peers) {
        LOG(p.second->id() << '\t'
            << p.second->PeerStatistics::findNodeCounter());
    }
}

int main(void) {
    Swarm& swarm = Swarm::getInstance();
    swarm.init(SIM, 10);
    swarm.addTaskAfter(0, [&swarm]
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

    swarm.addTaskAfter(1*60000, [&swarm]
    {
        calculateStatistic(swarm);
    });

    swarm.run();
    LOG("simulation done");
    return 0;
}



