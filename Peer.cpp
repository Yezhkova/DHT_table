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
	PeerStatistics::setPingCounter(PeerStatistics::ping() + 1);
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
	, const ID& senderId
	, const ID& queriedId) {
	// sender side
	Swarm::getInstance().addTaskAfter(m_packetTime, [responserId = recipientId
		, queriedId
		, senderId] 
		{
			// responser side
			if (auto responser = Swarm::getInstance().getPeer(responserId);
				responser != nullptr)
			{
				responser->receiveFindNode(senderId, queriedId);
			}
			else
			{
				LOG("sendFindNode Warning: the recipient peer "
					<< std::hex << responserId << " does not exist");
			}
		});

	PeerStatistics::incFindNodeCounter();
	PeerStatistics::incPacketCounter();

}

void Peer::receiveFindNode(const ID& requesterId
	, const ID& queriedId) {
	// responser side
	std::vector<ID> ids = m_node.receiveFindNode(requesterId, queriedId
		, boost::chrono::system_clock::now());

	Swarm::getInstance().addTaskAfter(m_packetTime, [requesterId
		, queriedId
		, ids
		, responserId = id()]
		{
			// requester side
			auto recipient = Swarm::getInstance().getPeer(requesterId);
			if (recipient != nullptr) {
				recipient->receiveFindNodeResponse(queriedId, ids, responserId);
			}
			else {
				LOG("receiveFindNode Warning: the recipient peer does not exist");
			}
		});

	PeerStatistics::incPacketCounter();
}

//void Peer::sendFindNodeResponse(const ID& requesterId
//	, const ID& queriedId
//	, std::vector<ID> idsFound) {
//	// receiver side
//	Swarm::getInstance().addTaskAfter(m_packetTime, [requesterId
//		, queriedId
//		, idsFound
//		, id = id()] 
//		{
//			// sender/receiver side
//			auto requester = Swarm::getInstance().getPeer(requesterId);
//			if (requester != nullptr) {
//				requester->receiveFindNodeResponse(queriedId, idsFound, id);
//			}
//			else {
//				LOG("sendFindNodeResponse Warning: the recipient peer does not exist");
//			}
//		});
//}

void Peer::receiveFindNodeResponse(const ID& queriedId
	, std::vector<ID> ids
	, const ID& responserId)
{
	m_node.receiveFindNodeResponse(queriedId, ids, responserId);
}

void Peer::onFindNodeResponse(bool find)
{
	//Swarm::getInstance().addTaskAfter(0, [this] {
	//	for (int i = 0; i < Swarm::getInstance().peers().size() * 0.3; ++i) {
	//		auto it = Swarm::getInstance().peers().begin();
	//		std::uniform_int_distribution<int> range(0
	//			, Swarm::getInstance().peers().size() - 1);
	//		int randomPeerNumber = range(s_randomGenerator);
	//		std::advance(it, randomPeerNumber);
	//		sendPing(it->second->id());
	//	}
	//	});
}

void Peer::onPacketReceived()
{

}
void Peer::onPacketSent()
{

}

