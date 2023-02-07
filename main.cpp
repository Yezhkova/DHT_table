#include "SwarmSimulator.h"
#include "Utils.h"
#include "iostream"

#define TCP true
#define SIM false

int main(void) {

//    SwarmSimulator simulator;
//    simulator.init(SIM, 3);
//    Swarm swarm = Swarm::getInstace();
//    LOG("must be 4: " << swarm.peers().size());
//    for(auto& e: swarm.peers())
//    {
//        LOG(e.first << ' ' << &e.second << '\n');
//    }
//    auto bootPeer = swarm.bootstrapNode();
//    LOG(bootPeer->node().id());
//    if(Swarm::getInstace().getPeer(ID()) != nullptr) {
//        LOG(" v ");
//    } else {
//        LOG(" f ");
//    }



    SwarmSimulator simulator;
    simulator.init(SIM, 5);
    simulator.addTask([&simulator]
    {
        Swarm swarm = Swarm::getInstace();
        auto peers = swarm.peers();

        for(auto & peer1 : peers)
        {
            LOG(peer1.second->id());
            peer1.second->start(swarm.bootstrapNode()->id());
        }


        /*
        for(auto & peer1 : peers)
        {
            std::vector<Bucket> buckets = peer1.second->node().bucketMap().nonEmptyBuckets();
            LOG(std::string(peer1.first) << ' ' << buckets.size());
            auto Buckets = peer1.second->node().bucketMap().map();
            LOG("buckets in map: " << Buckets.size());
            for(auto &e : Buckets)
            {
                LOG("nodes in bucket: " << e.second.bucket().size());
                for(auto & ee : e.second.bucket()){
                    LOG('i');
                    LOG(std::string(ee.first));
                }
            }
        }
        std::vector<Bucket> buckets = swarm.bootstrapNode()->node().bucketMap().nonEmptyBuckets();
        LOG(std::string(swarm.bootstrapNode()->node().id()) << ' ' << buckets.size());


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

//                if(peer2 != peer1) {
//                    swarm.ping(peer1.second->id(), peer2.second->id());
//                }

//                Peer2.second->ping(*Peer1.second);
            // simulatorRef.addTask([&] (SwarmSimulator & simulatorRef) {Peer2.ping(Peer1);}); // for TCP simulator

        }

        BucketMap bootMap = swarm.bootstrapNode()->node().bucketMap();
        LOG("bootMap: " << bootMap.size());
        for(auto & e : bootMap.map())
        {
            LOG("depth: " << e.first);
            auto bucket = e.second.bucket();
            for(auto IdNode: bucket)
            {
                LOG(std::string(IdNode.first));
            }
        }

        */


    });
    simulator.run();
    LOG("simulation done");
    return 0;
}



