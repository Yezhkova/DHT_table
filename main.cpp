#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false
#define MINUTES 60000.0
#define SWARM_SIZE 1000

void calculateStatistic(const Swarm& swarm) {
    LOG("------------------------------------calculateStatistic-------------------------");
    Swarm& s = Swarm::getInstance();
    auto peers = s.peers();

    std::map<int, int> findNodeStat;
    std::map<int, int> receiveFindNodeStat;
    std::map<int, int> packetsStat;

    for(auto& p : peers) {
        ++findNodeStat[p.second->PeerStatistics::findNode()];
        ++receiveFindNodeStat[p.second->PeerStatistics::receiveFindNode()];
        ++packetsStat[p.second->PeerStatistics::packets()];
    }

    LOG("findNode");
    for (auto& i : findNodeStat) {
        LOG(i.first << ' ' << i.second);
    }

    LOG("receiveFindNode");
    for (auto& i : receiveFindNodeStat) {
        LOG(i.first << ' ' << i.second);
    }

    LOG("packets");
    for (auto& i : packetsStat) {
        LOG(i.first << ' ' << i.second);
    }
}

int main(void) {
    Swarm& swarm = Swarm::getInstance();
    swarm.init(SIM, SWARM_SIZE);
    swarm.addTaskAfter(0, [&swarm]
    {
        auto peers = swarm.peers();
        for(auto & peer1 : peers)
        {
            //LOG(peer1.second->id() << " enters the swarm, " << peer1.second->packetTime());
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



