#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"
#include <boost/chrono.hpp>

#define MILLISEC_IN_SEC 1000.0

std::mt19937 Peer::s_randomGenerator;

Peer::Peer(ID id, IKademliaTransportProtocol& protocol, bool useTcp)
	: m_node(id, protocol, *this) {
	std::uniform_int_distribution<int> range(1, 100);

	// random number from 1 to 100 ms; data speed
	m_packetTime = range(s_randomGenerator) / MILLISEC_IN_SEC;
}

ID Peer::id() {
	return m_node.id();
}

Node& Peer::node() {
	return m_node;
}

NodeInfo& Peer::info() {
	return m_node.nodeInfo();
}

EventQueue::Interval Peer::packetTime() const {
	return m_packetTime;
}

void Peer::randomize() {
	m_node.randomizeId();
}

ID Peer::pickRandomPeer() {
    auto it = Swarm::getInstance().peers().begin();
    std::uniform_int_distribution<int> range(0
                   , Swarm::getInstance().peers().size() - 1);
    int randomPeerNumber = range(s_randomGenerator);
    std::advance(it, randomPeerNumber);
    return it->second->id();
}

bool Peer::addNode(const ID& id) {
	if (m_node.id() != id) {
		return m_node.addNode(id);
	}
	return false;
}

void Peer::start(const ID& bootstrapId) {
	addNode(bootstrapId);
    sendFindNode(bootstrapId, m_node.id(), m_node.id());
}

void Peer::sendPing(const ID& queriedId) {
	Swarm::getInstance().addTaskAfter(m_packetTime, [this, queriedId] {
		auto recipient = Swarm::getInstance().getPeer(queriedId);
		if (recipient != nullptr) {
			recipient->receivePing(m_node.id());
		}
		else {
			LOG("sendPing Warning: the recipient peer does not exist");
		}
		});

	PeerStatistics::incPingCounter();
}

void Peer::receivePing(const ID& queryingId) {
	Swarm::getInstance().addTaskAfter(m_packetTime, [this, queryingId] {
		auto recipient = Swarm::getInstance().getPeer(queryingId);
		if (recipient != nullptr) {
			recipient->receivePingResponse(m_node.id());
		}
		else {
			LOG("receivePing Warning: the recipient peer does not exist");
		}
		});
}

bool Peer::receivePingResponse(const ID& queriedId) {
	Swarm::getInstance().addTaskAfter(m_packetTime, [queriedId] {
		auto recipient = Swarm::getInstance().getPeer(queriedId);
		if (recipient != nullptr) {
			LOG(queriedId << " is alive!");
		}
		else {
			LOG("receivePing Warning: the recipient peer does not exist");
		}
		});
	return true;
}

void Peer::sendFindNode(const ID& recipientId
                        , const ID& requestorId
                        , const ID& queriedId) {
    // sender side
    Swarm::getInstance().addTaskAfter(m_packetTime, [responserId = recipientId
		, queriedId
        , requestorId
		, this]
		{
			// responser side
			if (auto responser = Swarm::getInstance().getPeer(responserId);
				responser != nullptr)
			{
                responser->receiveFindNode(requestorId, queriedId);
				Swarm::getInstance().getPeer(queriedId)->PeerStatistics::incFindNodeCounter();
				PeerStatistics::incPacketCounter();
			}
			else
			{
				LOG("sendFindNode Warning: the recipient peer "
					<< std::hex << responserId << " does not exist");
			}
		});
}

void Peer::receiveFindNode(const ID& requesterId
    , const ID& queriedId) {
	// responser side
    std::vector<ID> ids = m_node.receiveFindNode(requesterId, queriedId);

    Swarm::getInstance().addTaskAfter(m_packetTime, [requesterId
		, queriedId
		, ids
		, this]
		{
            // requester side
            auto recipient = Swarm::getInstance().getPeer(requesterId);
            if (recipient != nullptr) {
                recipient->receiveFindNodeResponse(queriedId, ids, id());
				PeerStatistics::incPacketCounter();
			}
			else {
				LOG("receiveFindNode Warning: the recipient peer does not exist");
			}
		});
}

void Peer::receiveFindNodeResponse(const ID& queriedId
	, std::vector<ID> ids
	, const ID& responserId)
{
	m_node.receiveFindNodeResponse(queriedId, ids, responserId);

	PeerStatistics::incPacketCounter();
}

void Peer::onFindNodeResponse(bool find)
{
	if (find) {
        // LOG("node found");
    }
	else {
//        LOG("---bad node: " << id());
	}
}

void Peer::onPacketReceived()
{

}
void Peer::onPacketSent()
{

}

void Peer::onBootstrap() {
    Swarm::getInstance().addTaskAfter(m_packetTime, [this] {
        for (int i = 0; i < 2; ++i) {
            ID recipientId = pickRandomPeer();
            ID queriedId = pickRandomPeer();
            sendFindNode(recipientId, id(), queriedId);
        }
    });
}
