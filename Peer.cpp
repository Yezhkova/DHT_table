#include "Peer.h"
#include "Utils.h"
#include "Swarm.h"
#include <boost/chrono.hpp>
#include "Swarm.h"


#define MILLISEC_IN_SEC 1000.0

std::mt19937 Peer::s_randomGenerator;
//uint64_t Peer::s_label = 0;


Peer::Peer(ID id, bool useTcp)
    : m_node(id, *this, *this, *this) {
    std::uniform_int_distribution<int> range(20, 30);
    m_packetTime = range(s_randomGenerator) / MILLISEC_IN_SEC;
//    m_label = ++s_label;
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
    m_node.onPingStart(queriedId);
    Swarm::getInstance().addTaskAfter(m_packetTime, [queriedId, id = id()] {
		auto recipient = Swarm::getInstance().getPeer(queriedId);
		if (recipient != nullptr) {
            recipient->receivePing(id);
		}
		else {
			LOG("sendPing Warning: the recipient peer does not exist");
		}
    });

    PeerStatistics::incPacketCounter();
    PeerStatistics::incPingCounter();
}

void Peer::receivePing(const ID& requestorId) {
    bool online = m_node.receivePing(requestorId);

    Swarm::getInstance().addTaskAfter(m_packetTime, [requestorId
                                                    , online
                                                    , id = id()] {
        auto requestor = Swarm::getInstance().getPeer(requestorId);
        if (requestor != nullptr) {
            requestor->receivePingResponse(online, id);
		}
		else {
			LOG("receivePing Warning: the recipient peer does not exist");
		}
		});

    PeerStatistics::incPacketCounter();
    PeerStatistics::incPingCounter();
}

void Peer::receivePingResponse(bool online, const ID& queriedId) {
    Swarm::getInstance().addTaskAfter(m_packetTime, [online, queriedId, this] {
        m_node.receivePingResponse(online, queriedId);
    });
}

void Peer::sendFindNode(const ID& recipientId
                        , const ID& initiatorId
                        , const ID& queriedId) {
    m_node.onFindNodeStart(queriedId);
    // sender side
    Swarm::getInstance().addTaskAfter(m_packetTime, [responserId = recipientId
		, queriedId
        , initiatorId, this]
		{	
            if (m_node.bucketMap().containsNode(queriedId))
            {
                receiveFindNodeResponse(queriedId, { queriedId }, id());
                PeerStatistics::incPacketCounter();
                return;
            }
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

    PeerStatistics::incPacketCounter();
    PeerStatistics::incFindNodeCounter();
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
    PeerStatistics::incFindNodeCounter();
}

void Peer::receiveFindNodeResponse(const ID& queriedId
	, std::vector<ID> ids
	, const ID& responserId)
{
    Swarm::getInstance().addTaskAfter(m_packetTime, [queriedId
                                                    , ids
                                                    , responserId
                                                    , this]{
        m_node.receiveFindNodeResponse(queriedId, ids, responserId);
    });

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

void Peer::findRandomNodes(int nodeNumber) {
    Swarm::getInstance().addTaskAfter(m_packetTime, [this, nodeNumber] {
        for (int i = 0; i < nodeNumber; ++i) {
            ID queriedId = pickRandomPeer();
            //TODO
            ID recipientId = m_node.findClosestNodes(1, queriedId)[0];
//            LOG(id() << " is looking for " << queriedId << " in " << recipientId << std::endl);
            sendFindNode(recipientId, id(), queriedId);
        }
    });
}

void Peer::startTimer(EventQueue::Interval duration, std::function<void ()> F) {
    Swarm::getInstance().addTaskAfter(duration, F);
}

