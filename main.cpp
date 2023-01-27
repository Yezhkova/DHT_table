#include "SwarmSimulator.h"
#include "Utils.h"

#define TCP true
#define SIM false

int main(void) {

    auto simulator = std::make_shared<SwarmSimulator>(SIM, 5);
    simulator->addTask([&simulator]
    {
        Swarm swarm = simulator->getSwarm();
        auto peers = swarm.peers();
        for(auto & peer1 : peers)
        {
            peer1.second->bootstrap(swarm.bootstrapNode());
        }
        for(auto & peer2 : peers)
        {
            LOG(std::string(peer2.second->id()) << " stores some nodes:\n");
            auto bucketMap = peer2.second->node().bucketMap();
            LOG("bucketMap: " << bucketMap.size());
            std::vector<Bucket> occ = bucketMap.nonEmptyBuckets();
            LOG("occ: " << occ.size());
            for(auto b : occ)
            {
                LOG("b: " << b.size());
                for(auto & IdNodePair : b.bucket())
                {
                    LOG(std::string(IdNodePair.first) << '\n');
                }
            }

            /*
//                if(peer2 != peer1) {
//                    swarm.ping(peer1.second->id(), peer2.second->id());
//                }

//                Peer2.second->ping(*Peer1.second);
            // simulatorRef.addTask([&] (SwarmSimulator & simulatorRef) {Peer2.ping(Peer1);}); // for TCP simulator
            */
        }

    });
    simulator->run();
    LOG("simulation done");
    return 0;
}



