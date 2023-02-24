#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false
#define MINUTES 60000.0

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
    swarm.init(SIM, 4);
    swarm.addTaskAfter(0, [&swarm]
    {
        auto peers = swarm.peers();
        for(auto & peer1 : peers)
        {
            LOG(peer1.second->id() << " enters the swarm, " << peer1.second->packetTime());
            if( peer1.second->id() != swarm.bootstrapNode()->id()){
                peer1.second->start(swarm.bootstrapNode()->id());
            }
        }
    });

    swarm.addTaskAfter(10 * MINUTES, [&swarm]
    {
        calculateStatistic(swarm);
    });
    swarm.run();
    LOG("simulation done");
    return 0;
}



