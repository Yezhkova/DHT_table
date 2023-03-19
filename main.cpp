#include "Swarm.h"
#include "Utils.h"
#include "Constants.h"
std::chrono::system_clock::time_point start;
std::chrono::system_clock::time_point end;

void Step() {
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    static int stepCounter = 0;
    Swarm& swarm = Swarm::getInstance();
    swarm.calculateStatistic(elapsed_seconds);

    auto peers = swarm.peers();
    for (auto& peer : peers)
    {
        peer.second->resetAllCounters();
    }
    for (auto& peer : peers)
    {
        peer.second->findRandomNodes(FIND_RANDOM_NODES);
    }

    swarm.addTaskAfter(10 * MINUTES, [&swarm] {
        if (++stepCounter > 100) {
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            swarm.calculateStatistic(elapsed_seconds);
            Swarm::getInstance().eventQueqe().removeAllEvents();
        }
        else {
            EX_LOG("stepCounter = " << stepCounter);
            Step();
        }
    });
}

int main(void) {
    start = std::chrono::system_clock::now();
    
	Swarm& swarm = Swarm::getInstance();
	swarm.init(SIM, SWARM_SIZE);
    swarm.addTaskAfter(0, [&swarm]
    {
        uint64_t i = 0;
        auto peers = swarm.peers();
        for (auto& peer : peers)
        {
            peer.second->node().setLabel(++i);
            LOG(peer.second->id() << " enters the swarm, " << std::dec << peer.second->packetTime());
            if (peer.second->id() != swarm.bootstrapNode()->id()) {
                peer.second->start(swarm.bootstrapNode()->id());
            }
        }
    });


    swarm.addTaskAfter(3 * MINUTES, [&swarm] {
         Step();
    });

	swarm.run();
    EX_LOG("simulation done");
    //auto end = std::chrono::system_clock::now();

    //std::chrono::duration<double> elapsed_seconds = end - start;
    //std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    //EX_LOG("finished computation at " << std::ctime(&end_time)
    //    << "elapsed time: " << elapsed_seconds.count() << "s");
	return 0;
}