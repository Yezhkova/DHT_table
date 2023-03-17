#include "Node.h"
#include "Swarm.h"
#include "Utils.h"
#include <optional>

#define CLOSEST_NODES 3
#define FIND_NODE_THRESHOLD 1000
#define PING_THRESHOLD 2
#define PING_INTERVAL 15
#define MINUTES 60

std::mt19937 Node::m_randomGenerator;


const ID& Node::id() const {
	return m_contact.m_id;
}

const BucketArray& Node::buckets() const {
	return m_BucketArray;
}

const Contact& Node::contact() const {
	return m_contact;
}

NodeInfo& Node::nodeInfo() {
	return m_info;
}

INodeEventHandler& Node::eventHandler() {
	return m_eventHandler;
}

void Node::randomizeId() {
	m_contact.randomize();
}

const uint64_t Node::label() const {
	return m_index;
}

void Node::setLabel(uint64_t label) {
	m_index = label;
}
// TODO: node offline mode not implemented

bool Node::addNode(const ID& newId) {
	if (this->id() != newId) {
		size_t bucketIdx = m_BucketArray.calcBucketIndex(newId);
		if (!m_BucketArray.bucketFull(bucketIdx)) {
			m_protocol.sendPing(newId);
			return m_BucketArray.addNode(newId, bucketIdx);
		}
		return false;
	}
	return false;
}

bool Node::removeNode(const ID& id) {
	return m_BucketArray.removeNode(id);
}

void Node::updateLastSeen(const ID& id
	, boost::chrono::system_clock::time_point time) {
	auto peerToUpdate = Swarm::getInstance().getPeer(id);
	if (peerToUpdate != nullptr) {
		peerToUpdate->info().updateLastSeen(time);
	}
}

bool operator==(const Node& l, const Node& r) {
	return l.m_contact.m_id == r.m_contact.m_id;
}

IDhtTransportProtocol& Node::protocol() {
	return m_protocol;
}

const ID& Node::pickRandomNode(const Bucket& bucket) const
{
	auto it = bucket.begin();
	std::uniform_int_distribution<int> range(0, bucket.size() - 1);
	int randomNodeNumber = range(m_randomGenerator);
	std::advance(it, randomNodeNumber);
	return it->m_id;
}

void Node::fill(int bucketIdx, std::set<const ID*>& outIds, int k)
{
	auto& bucket = m_BucketArray.getBucket(bucketIdx);
	if (bucket.size() == 0) return;

	if (bucket.size() < k) {
		for (auto& contact : bucket) {
			outIds.insert(&contact.id());
		}
		return;
	}

	while (outIds.size() < k) {
		auto& id = pickRandomNode(bucket);
		outIds.insert(&id);
	}
}

std::vector<const ID*> Node::findClosestNodes(int k, const ID& id)
{
	std::set<const ID*> res;
	// start with the bucket where ID could be
	int bucketIndex = m_BucketArray.calcBucketIndex(id);
	fill(bucketIndex, res, k);

	// not enough ids
	if (res.size() < k) {
		int nextBucketIndex = bucketIndex, prevBucketIndex = bucketIndex;
		size_t i = 1;
		for (; nextBucketIndex < BucketArray::g_treeSize && prevBucketIndex >= 0; ++i)
		{
			nextBucketIndex = bucketIndex + i;
			prevBucketIndex = bucketIndex - i;
			if (nextBucketIndex < 160) {
				fill(nextBucketIndex, res, k);
			}
			if (prevBucketIndex >= 0) {
				fill(prevBucketIndex, res, k);
			}
		}
		for (size_t j = i; res.size() < k && nextBucketIndex < BucketArray::g_treeSize; ++j)
		{
			nextBucketIndex = bucketIndex + j;
			if (nextBucketIndex < 160) {
				fill(nextBucketIndex, res, k);
			}
		}
		for (size_t j = i; res.size() < k && prevBucketIndex >= 0; ++j)
		{
			prevBucketIndex = bucketIndex - j;
			if (prevBucketIndex >= 0) {
				fill(prevBucketIndex, res, k);
			}
		}
	}
	std::vector<const ID*> ids(res.begin(), res.end());
	return ids;
}

std::vector<const ID*> Node::receiveFindNode(const ID& senderId
	, const ID& queriedId)
{
	addNode(senderId);
	if (auto* contact = m_BucketArray.getContactPtr(queriedId); contact != nullptr) {
		LOG("returning myself: "<< & queriedId);
		return { &contact->m_id };
	}
	else {
		return findClosestNodes(CLOSEST_NODES, queriedId);
	}
}

bool Node::receivePing(const ID& requestorId) {
	return m_online;
}

void Node::receivePingResponse(bool online
	, const ID& queriedId)
{
	if (auto it = m_pingMap.find(queriedId); it == m_pingMap.end()) {
		return;
	}

	if (!online && m_pingMap[queriedId] < PING_THRESHOLD)
	{
		m_protocol.sendPing(queriedId);
	}
	else {
		onPingEnd(online, queriedId);
	}
}

void Node::receiveFindNodeResponse(const ID& queriedId
	, const std::vector<const ID*>& ids
	, const ID& responserId)
{
	if (auto it = m_findNodeMap.find(queriedId); it == m_findNodeMap.end()) {
		return;
	}
	addNode(responserId);
	if (m_findNodeMap[queriedId] > FIND_NODE_THRESHOLD) {
		onFindNodeEnd(false, queriedId);
		return;
	}
	if (*ids[0] == queriedId) {
		onFindNodeEnd(true, queriedId);
	}
	else {
		//LOG(queriedId << " not found");
		for (auto id : ids) {
			m_protocol.sendFindNode(*id, this->id(), queriedId); // Swarm does this
		}
	}

}

void Node::onFindNodeStart(const ID& queriedId)
{
	m_findNodeMap[queriedId]++;
}

void Node::onFindNodeEnd(bool found, const ID& queriedId)
{
	m_findNodeMap.erase(queriedId);
	if (found) {
		m_eventHandler.onNodeFound();
	}
	else {
		m_eventHandler.onNodeNotFound();
	}
}

void Node::onPingStart(const ID& queriedId)
{
	m_pingMap[queriedId] = 1;
}

void Node::onPingEnd(bool online, const ID& queriedId)
{
	m_pingMap.erase(queriedId);
	if (!online) {
		removeNode(queriedId);
	}
	m_timerProtocol.startTimer(PING_INTERVAL * MINUTES, [this, queriedId] {
		m_protocol.sendPing(queriedId);
		});
}

void Node::setOffline() {
	m_online = false;
}

void Node::setOnline() {
	m_online = true;
}


