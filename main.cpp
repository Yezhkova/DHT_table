#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false
#define MINUTES 60
#define SWARM_SIZE 1000

std::chrono::system_clock::time_point start;

void Step() {
    static int stepCounter = 0;

    Swarm& swarm = Swarm::getInstance();
    swarm.calculateStatistic();

    auto peers = swarm.peers();
    for (auto& peer : peers)
    {
        peer.second->resetAllCounters();
    }
    for (auto& peer : peers)
    {
        peer.second->findRandomNodes(1);
    }

    swarm.addTaskAfter(10 * MINUTES, [&swarm] {
        if (++stepCounter > 5) {
            swarm.calculateStatistic();
            Swarm::getInstance().eventQueqe().removeAllEvents();
        }
        else {
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
//            LOG(peer.second->id() << " enters the swarm, " << std::dec << peer.second->packetTime());
            if (peer.second->id() != swarm.bootstrapNode()->id()) {
                peer.second->start(swarm.bootstrapNode()->id());
            }
        }
    });


    swarm.addTaskAfter(3 * MINUTES, [&swarm]
    {
         Step();
    });

	swarm.run();
    EX_LOG("simulation done");
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
        << "elapsed time: " << elapsed_seconds.count() << "s"
        << std::endl;
	return 0;
}

// TODO: statistics - max Bucket size, mid(max) Bucket size
// TODO: profiling search for bottleneck
// TODO: pass timediff to calculateStatistic



