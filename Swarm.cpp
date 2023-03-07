#include "Swarm.h"
#include "Utils.h"

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
    // MOD
    m_bootstrapNode = std::make_shared<Peer>(ID().normalRandomize(), mode);
    m_peers[m_bootstrapNode->id()] = m_bootstrapNode;
    generateSwarm(mode, PeerNumber);
}

void Swarm::addTaskAfter(EventQueue::Interval duration, std::function<void ()> F) {
    m_eventQueqe.addTaskAt(duration, F);
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

void Swarm::calculateStatistic() {
    LOG("------------------------------------calculateStatistic-------------------------");
    std::map<int, int> findNodeStat;
    std::map<int, int> packetsStat;
    int deadNodesStat = 0;

    for (auto& p : m_peers) {
        ++findNodeStat[p.second->PeerStatistics::findNode()];
        ++packetsStat[p.second->PeerStatistics::packetsCnt()];
        deadNodesStat += p.second->PeerStatistics::failedFindNode();
    }

    LOG("number of operations --- number of peers");
    LOG("findNode");
    for (auto& i : findNodeStat) {
        LOG(std::dec << i.first << ' ' << i.second);
    }

//    LOG("packets");
//    for (auto& i : packetsStat) {
//        LOG(i.first << ' ' << i.second);
//    }

    LOG("dead nodes: " << deadNodesStat);

}

