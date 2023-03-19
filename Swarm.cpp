#include "Swarm.h"
#include "Utils.h"
#include "EventQueue.h"
#include "Constants.h"
EventQueue& Swarm::eventQueqe() {
    return m_eventQueqe;
}

Swarm& Swarm::getInstance()
{
    static Swarm swarm;
    return swarm;
}

void Swarm::init(bool mode, int PeerNumber)
{
    m_peers.clear();
    m_bootstrapNode.reset();
    m_bootstrapNode = std::make_shared<Peer>(ID(), mode);
    m_peers[m_bootstrapNode->id()] = m_bootstrapNode;
    generateSwarm(mode, PeerNumber);
}

void Swarm::addTaskAfter(EventQueue::Interval duration, std::function<void ()>&& F) {
    m_eventQueqe.addTaskAt(m_eventQueqe.currentTime() + duration, std::move(F));
}

void Swarm::run() {
    m_eventQueqe.run();
}

void Swarm::stop() {
    m_eventQueqe.removeAllEvents();
}

bool Swarm::tcp() {
    return m_useTcp;
}

std::shared_ptr<Peer>& Swarm::bootstrapNode() {
    return m_bootstrapNode;
}

const std::map<ID, std::shared_ptr<Peer>>& Swarm::peers() const {
    return m_peers;
}

std::shared_ptr<Peer> Swarm::getPeer(const ID& id) {
    auto it = m_peers.find(id);
    if(it != m_peers.end()) {
        return it->second;
    }
    assert(0);
    return nullptr;
}

void Swarm::generateSwarm(bool mode, size_t Peers)
{
    for(size_t i = 0; i < Peers; ++i)
    {
        ID id = createRandomId();
        m_peers[id] = std::make_shared<Peer>(id, mode);
    }
}

void Swarm::calculateStatistic(std::chrono::duration<double> elapsed_seconds) {
    EX_LOG("------------------------------------calculateStatistic-------------------------");
    EX_LOG("elapsed time: " << elapsed_seconds.count() << " s");
    EX_LOG("Swarm queue current time: " << Swarm::getInstance().eventQueqe().currentTime() / 60);
    uint64_t nodeNotFoundCounter = 0;
    uint64_t pingCounter = 0;
    uint64_t packetCounter = 0;
    uint64_t findNodeCounter = 0;
    uint64_t doneCounter = 0;
    size_t overallMaxContactsInBucket = 0;
    ID overallMCBId;
    uint64_t sumMaxContactsInBucket = 0;
    int medMaxContactsInBucket = 0;
    //std::vector<uint64_t> sizes(TREE_SIZE); // 160 zeroes

    for (auto& p : m_peers) {
        nodeNotFoundCounter += p.second->failedFindNodeCounter();
        findNodeCounter += p.second->findNodeCounter();
        packetCounter += p.second->packetsCounter();
        pingCounter += p.second->pingCounter();
        doneCounter += p.second->done();

        size_t TmpMaxContactsInBucket = 0;
        for (int i = 0; i < TREE_SIZE; ++i) {
            auto& bucket = p.second->node().buckets().data()[i];
            //sizes[i] += bucket.size();
            if (bucket.size() > TmpMaxContactsInBucket) {
                TmpMaxContactsInBucket = bucket.size();
            }
        }
        sumMaxContactsInBucket += TmpMaxContactsInBucket;
        if (TmpMaxContactsInBucket > overallMaxContactsInBucket) {
            overallMaxContactsInBucket = TmpMaxContactsInBucket;
            overallMCBId = p.second->id();
        }
    }

    EX_LOG("dead nodes: " << std::dec << nodeNotFoundCounter);
    EX_LOG("find node: " << findNodeCounter);
    EX_LOG("packets: " << packetCounter);
    EX_LOG("ping: " << pingCounter);
    EX_LOG("done: " << doneCounter);
    EX_LOG("maximum bucket size: " << overallMaxContactsInBucket << " (found in " << overallMCBId << ')');
    EX_LOG("AVG(maximum bucket size): " << std::dec << sumMaxContactsInBucket / m_peers.size());
    /*EXLOG("average BucketArray load: ");
    for (int i = 0; i < BucketArray::g_treeSize; ++i) {
        EX_LOG(i << ' ' << sizes[i] / m_peers.size());
    }*/
    EX_LOG("-------------------------------------End Of Statistic-------------------------");
}

