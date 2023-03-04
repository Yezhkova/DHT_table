#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"
#include <boost/chrono.hpp>

#define MILLISEC_IN_SEC 1000.0

std::mt19937 Peer::s_randomGenerator;
uint64_t Peer::s_label = 0;


Peer::Peer(ID id, IKademliaTransportProtocol& protocol, bool useTcp)
	: m_node(id, protocol, *this) {
    std::uniform_int_distribution<int> range(1, 100);
    m_packetTime = range(s_randomGenerator) / MILLISEC_IN_SEC;
    m_label = ++s_label;
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

uint64_t Peer::label() const {
    return m_label;
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
                        , const ID& initiatorId
                        , const ID& queriedId) {
    m_node.onFindNodeStart(queriedId);
    // sender side
    Swarm::getInstance().addTaskAfter(m_packetTime, [responserId = recipientId
		, queriedId
        , initiatorId]
		{	
//            if (m_node.bucketMap().containsNode(queriedId))
//            {
//                receiveFindNodeResponse(queriedId, { queriedId }, id());
//                return;
//            }
            // responser side
			if (auto responser = Swarm::getInstance().getPeer(responserId);
				responser != nullptr)
			{
                responser->receiveFindNode(initiatorId, queriedId);
			}
			else
			{
				LOG("sendFindNode Warning: the recipient peer "
					<< std::hex << responserId << " does not exist");
			}
		});

    Swarm::getInstance().getPeer(queriedId)->PeerStatistics::incFindNodeCounter();
    PeerStatistics::incPacketCounter();
}

void Peer::receiveFindNode(const ID& initiatorId
                          , const ID& queriedId) {
	// responser side
    std::vector<ID> ids = m_node.receiveFindNode(initiatorId, queriedId);

    Swarm::getInstance().addTaskAfter(m_packetTime, [initiatorId
		, queriedId
		, ids
        , id = id()]
		{
            // requester side
            if (auto initiator = Swarm::getInstance().getPeer(initiatorId);
                    initiator != nullptr)
            {
                initiator->receiveFindNodeResponse(queriedId, ids, id);
			}
			else {
				LOG("receiveFindNode Warning: the recipient peer does not exist");
			}
		});

    PeerStatistics::incPacketCounter();
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

void Peer::onPingResponse(bool find) {

}
void Peer::onPacketReceived() {

}
void Peer::onPacketSent() {

}

void Peer::onBootstrap() {
    Swarm::getInstance().addTaskAfter(m_packetTime, [this] {
        for (int i = 0; i < 2; ++i) {
			ID queriedId = pickRandomPeer();
			//if (queriedId != ID()) {
				ID recipientId = m_node.findClosestNodes(1, queriedId)[0];
				//LOG(id() << " is looking for " << queriedId << " in " << recipientId << std::endl);
				sendFindNode(recipientId, id(), queriedId);
			//}
        }
    });
}
