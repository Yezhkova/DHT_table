#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false
#define MINUTES 60000.0
#define SWARM_SIZE 36

void calculateStatistic(const Swarm& swarm) {
	LOG("------------------------------------calculateStatistic-------------------------");
	Swarm& s = Swarm::getInstance();
	auto peers = s.peers();

	std::map<int, int> findNodeStat;
	std::map<int, int> receiveFindNodeStat;
	std::map<int, int> packetsStat;
	int deadNodesStat = 0;

	for (auto& p : peers) {
		++findNodeStat[p.second->PeerStatistics::findNode()];
		++receiveFindNodeStat[p.second->PeerStatistics::receiveFindNode()];
		++packetsStat[p.second->PeerStatistics::packets()];
		deadNodesStat += p.second->PeerStatistics::failedFindNode();
	}
	for (auto& p : peers) {
		if (p.second->findNode() == 1093) LOG(p.second->id());
	}
	LOG("number of operations --- number of peers");
	LOG("findNode");
	for (auto& i : findNodeStat) {
		LOG(std::dec << i.first << ' ' << i.second);
	}

	LOG("receiveFindNode");
	for (auto& i : receiveFindNodeStat) {
		LOG(i.first << ' ' << i.second);
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
				//LOG(peer1.second->id() << " enters the swarm, " << peer1.second->packetTime());
				if (peer.second->id() != swarm.bootstrapNode()->id()) {
					peer.second->start(swarm.bootstrapNode()->id());
				}
			}
		});

	swarm.addTaskAfter(10 * MINUTES, [&swarm]
		{
			calculateStatistic(swarm);
		});

	swarm.addTaskAfter(20 * MINUTES, [&swarm]
		{
			auto peers = swarm.peers();
			for (auto& peer : peers)
			{
				peer.second->resetFindNodeCounter();
				peer.second->resetPacketCounter();
			}
			for (auto& peer : peers)
			{
				peer.second->onBootstrap();
			}
		});

	swarm.addTaskAfter(40 * MINUTES, [&swarm]
		{
			calculateStatistic(swarm);
		});

	swarm.run();
	LOG("simulation done");
	return 0;
}



