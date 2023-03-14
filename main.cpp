#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false
#define MINUTES 60
#define SWARM_SIZE 1000

int main(void) {
    auto start = std::chrono::system_clock::now();
    
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
        
        swarm.calculateStatistic();

        swarm.addTaskAfter(0 * MINUTES, [&swarm]
        {
            auto peers = swarm.peers();
            for (auto& peer : peers)
            {
                peer.second->resetFindNodeCounter();
                peer.second->resetPacketCounter();
                peer.second->resetReceiveFindNodeCounter();
                peer.second->resetFailedNode();
                peer.second->resetDone();
            }
            for (auto& peer : peers)
            {
                peer.second->findRandomNodes(3);
            }

        });

        swarm.addTaskAfter(10 * MINUTES, [&swarm]
        {
            swarm.calculateStatistic();
            Swarm::getInstance().eventQueqe().removeAllEvents();
        });
        

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



