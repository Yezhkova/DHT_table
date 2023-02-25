#include "Swarm.h"
#include "Utils.h"

const EventQueue& Swarm::eventQueqe() const {
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

    m_bootstrapNode = std::make_shared<Peer>(ID(), *this, mode);
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
    m_eventQueqe.stop();
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
        m_peers[id] = std::make_shared<Peer>(id, *this, mode);
    }
}

void Swarm::sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId)
{

}

void Swarm::sendPingResponse(const ID & queryingId, const ID & queriedId)
{

}

void Swarm::sendFindNode(const ID & recipientId, const ID & requesterId, const ID & queriedId)
{
    auto recipient = Swarm::getInstance().getPeer(recipientId);
    if (recipient != nullptr) {
        recipient->sendFindNode(recipientId, requesterId, queriedId);
    }
    else {
        LOG("sendFindNode Warning: the recipient peer "
            << std::hex << recipientId << " does not exist");
    }
}

void Swarm::sendFindNodeResponse(const ID & myId, const ID & queriedId)
{

}


