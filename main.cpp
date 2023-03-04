#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false
#define MINUTES 60000.0
#define SWARM_SIZE 150

void calculateStatistic(const Swarm& swarm) {
	LOG("------------------------------------calculateStatistic-------------------------");
	Swarm& s = Swarm::getInstance();
	auto peers = s.peers();

	std::map<int, int> findNodeStat;
	std::map<int, int> packetsStat;
	int deadNodesStat = 0;

	for (auto& p : peers) {
		++findNodeStat[p.second->PeerStatistics::findNode()];
        ++packetsStat[p.second->PeerStatistics::packetsCnt()];
		deadNodesStat += p.second->PeerStatistics::failedFindNode();
	}

	LOG("number of operations --- number of peers");
	LOG("findNode");
	for (auto& i : findNodeStat) {
		LOG(std::dec << i.first << ' ' << i.second);
	}

	LOG("packets");
	for (auto& i : packetsStat) {
		LOG(i.first << ' ' << i.second);
	}

	LOG("dead nodes: " << deadNodesStat);
}

int main(void) {
	Swarm& swarm = Swarm::getInstance();
	swarm.init(SIM, SWARM_SIZE);
    swarm.addTaskAfter(0, [&swarm]
    {
        auto peers = swarm.peers();
        for (auto& peer : peers)
        {
//            LOG(peer.second->id() << " enters the swarm, " << std::dec << peer.second->label());
            if (peer.second->id() != swarm.bootstrapNode()->id()) {
                peer.second->start(swarm.bootstrapNode()->id());
            }
        }
    });


    swarm.addTaskAfter(50 * MINUTES, [&swarm]
    {
        calculateStatistic(swarm);
        Swarm::getInstance().eventQueqe().removeAllEvents();

        swarm.addTaskAfter(56 * MINUTES, [&swarm]
        {
            auto peers = swarm.peers();
            for (auto& peer : peers)
            {
                peer.second->resetFindNodeCounter();
                peer.second->resetPacketCounter();
                peer.second->resetReceiveFindNodeCounter();
                peer.second->resetFailedNode();
            }
            for (auto& peer : peers)
            {
                peer.second->onBootstrap();
            }
        });

        swarm.addTaskAfter(180 * MINUTES, [&swarm]
        {
            calculateStatistic(swarm);
        });
    });

	swarm.run();
	LOG("simulation done");
	return 0;
}



