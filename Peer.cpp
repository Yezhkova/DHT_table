#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"
#include <boost/chrono.hpp>
#include "Swarm.h"

#define MILLISEC_IN_SEC 1000.0
//#define FAST_MODE

std::mt19937 Peer::s_randomGenerator;

Peer::Peer(ID id, bool useTcp)
	: m_node(id, *this, *this, *this) {
	std::uniform_int_distribution<int> range(20, 30);
	m_packetTime = range(s_randomGenerator) / MILLISEC_IN_SEC;
}

const ID& Peer::id() const {
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

void Peer::start(const ID& bootstrapId) {
	m_node.addNode(bootstrapId);
	sendFindNode(bootstrapId, m_node.id(), m_node.id());
}

void Peer::sendPing(const ID& queriedId) {
	m_node.onPingStart(queriedId);
	const ID& id = this->id();
#ifndef FAST_MODE
	Swarm::getInstance().addTaskAfter(m_packetTime, [queriedId, id] {
#endif
		auto recipient = Swarm::getInstance().getPeer(queriedId);
		if (recipient != nullptr) {
			recipient->receivePing(id);
		}
		else {
			LOG("sendPing Warning: the recipient peer does not exist");
		}

#ifndef FAST_MODE
		});
#endif
	PeerStatistics::incPacketCounter();
	PeerStatistics::incPingCounter();
}

void Peer::receivePing(const ID& requestorId) {
	bool online = m_node.receivePing(requestorId);
	const ID& id = this->id();
#ifndef FAST_MODE
	Swarm::getInstance().addTaskAfter(m_packetTime, [requestorId, online, id] {
#endif
		auto requestor = Swarm::getInstance().getPeer(requestorId);
		if (requestor != nullptr) {
			requestor->receivePingResponse(online, id);
		}
		else {
			LOG("receivePing Warning: the recipient peer does not exist");
		}
#ifndef FAST_MODE
		});
#endif
	PeerStatistics::incPacketCounter();
	PeerStatistics::incPingCounter();
}

void Peer::receivePingResponse(bool online, const ID& queriedId) {
#ifndef FAST_MODE
	Swarm::getInstance().addTaskAfter(m_packetTime, [online, queriedId, this] {
#endif
		m_node.receivePingResponse(online, queriedId);
#ifndef FAST_MODE
		});
#endif
}

void Peer::sendFindNode(const ID& recipientId, const ID& initiatorId, const ID& queriedId) {
	m_node.onFindNodeStart(queriedId);
	// sender side
#ifndef FAST_MODE
	Swarm::getInstance().addTaskAfter(m_packetTime, [recipientId, queriedId, initiatorId, this] {
#endif
		if (auto* contact = m_node.buckets().getContactPtr(queriedId); contact != nullptr)
		{
			receiveFindNodeResponse(queriedId, { &contact->m_id }, id());
			PeerStatistics::incPacketCounter();
			return;
		}
		// responser side
		if (auto responser = Swarm::getInstance().getPeer(recipientId);
			responser != nullptr)
		{
			responser->receiveFindNode(initiatorId, queriedId);
		}
		else
		{
			LOG("sendFindNode Warning: the recipient peer "
				<< std::hex << responserId << " does not exist");
		}
#ifndef FAST_MODE
		});
#endif
	PeerStatistics::incPacketCounter();
	PeerStatistics::incFindNodeCounter();
}

void Peer::receiveFindNode(const ID& initiatorId, const ID& queriedId) {
	// responser side
	std::vector<const ID*> ids = m_node.receiveFindNode(initiatorId, queriedId);
	const ID& id = this->id();
#ifndef FAST_MODE
	Swarm::getInstance().addTaskAfter(m_packetTime, [initiatorId, queriedId, ids = std::move(ids), id]{
#endif
		// requester side
		if (auto initiator = Swarm::getInstance().getPeer(initiatorId);
			initiator != nullptr) {
			initiator->receiveFindNodeResponse(queriedId, ids, id);
		}
		else {
			LOG("receiveFindNode Warning: the recipient peer does not exist");
		}
#ifndef FAST_MODE
		});
#endif
	PeerStatistics::incPacketCounter();
	PeerStatistics::incFindNodeCounter();
}

void Peer::receiveFindNodeResponse(const ID& queriedId
	, const std::vector<const ID*>& ids
	, const ID& responserId)
{
#ifndef FAST_MODE
	Swarm::getInstance().addTaskAfter(m_packetTime, [queriedId, ids, responserId, this] {
#endif
		 m_node.receiveFindNodeResponse(queriedId, ids, responserId);
#ifndef FAST_MODE
	});
#endif
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

void Peer::onNodeNotFound() {
	incFailedFindNode();
}

void Peer::onNodeFound() {
	setDone();
}

void Peer::findRandomNodes(int nodeNumber) {
#ifndef FAST_MODE
	Swarm::getInstance().addTaskAfter(m_packetTime, [this, nodeNumber] {
#endif
		for (int i = 0; i < nodeNumber; ++i) {
			ID queriedId = pickRandomPeer();
			ID recipientId = m_node.findClosestNode(queriedId);
			sendFindNode(recipientId, id(), queriedId);
		}
#ifndef FAST_MODE
		});
#endif
}

void Peer::startTimer(EventQueue::Interval duration, std::function<void()>&& F) {
	Swarm::getInstance().addTaskAfter(duration, std::move(F));
}