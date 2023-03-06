#include "Swarm.h"
#include "Utils.h"

#define TCP true
#define SIM false
#define MINUTES 60000.0
#define SWARM_SIZE 50

int main(void) {
	Swarm& swarm = Swarm::getInstance();
	swarm.init(SIM, SWARM_SIZE);
    swarm.addTaskAfter(0, [&swarm]
    {
        uint64_t i = 0;
        auto peers = swarm.peers();
        for (auto& peer : peers)
        {
            peer.second->node().setLabel(++i);
            LOG(peer.second->id() << " enters the swarm, " << std::dec << peer.second->node().label());
            if (peer.second->id() != swarm.bootstrapNode()->id()) {
                peer.second->start(swarm.bootstrapNode()->id());
            }
        }
    });


//    swarm.addTaskAfter(50 * MINUTES, [&swarm]
//    {
//        calculateStatistic(swarm);
//        Swarm::getInstance().eventQueqe().removeAllEvents();

//        swarm.addTaskAfter(56 * MINUTES, [&swarm]
//        {
//            auto peers = swarm.peers();
//            for (auto& peer : peers)
//            {
//                peer.second->resetFindNodeCounter();
//                peer.second->resetPacketCounter();
//                peer.second->resetReceiveFindNodeCounter();
//                peer.second->resetFailedNode();
//            }
//            for (auto& peer : peers)
//            {
//                peer.second->onBootstrap();
//            }
//        });

//        swarm.addTaskAfter(180 * MINUTES, [&swarm]
//        {
//            calculateStatistic(swarm);
//        });
//    });

	swarm.run();
	LOG("simulation done");
	return 0;
}



