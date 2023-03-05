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

    m_bootstrapNode = std::make_shared<Peer>(ID(), *this, *this, mode);
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
        m_peers[id] = std::make_shared<Peer>(id, *this, *this, mode);
    }
}

void Swarm::sendPingInSwarm(const ID & requestorId
                            , const ID & queriedId)
{
    if (auto requestor = Swarm::getInstance().getPeer(requestorId);
        requestor != nullptr)
    {
        requestor->sendPing(queriedId);
    }
    else {
        LOG("sendPing Warning: the recipient peer "
            << std::hex << requestorId << " does not exist");
    }
}


void Swarm::sendFindNodeInSwarm(const ID & recipientId
                         , const ID & initiatorId
                         , const ID & queriedId)
{

    if (auto requestor = Swarm::getInstance().getPeer(initiatorId);
        requestor != nullptr)
    {
       requestor->sendFindNode(recipientId, initiatorId, queriedId);
    }
    else {
        LOG("sendFindNode Warning: the recipient peer "
            << std::hex << initiatorId << " does not exist");
    }
}


