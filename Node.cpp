#include "Node.h"
#include "Swarm.h"
#include "Utils.h"
#include "Constants.h"
#include <optional>

#define OPT_MODE

std::mt19937 Node::m_randomGenerator;

void Node::randomizeId() {
	m_contact.randomize();
}

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

bool operator==(const Node& l, const Node& r) {
	return l.m_contact.m_id == r.m_contact.m_id;
}

const ID& Node::pickRandomNode(const Bucket& bucket) const
{
	auto it = bucket.begin();
	std::uniform_int_distribution<int> range(0, bucket.size() - 1);
	int randomNodeNumber = range(m_randomGenerator);
	std::advance(it, randomNodeNumber);
	return it->m_id;
}

void Node::fill(int bucketIdx, std::vector<const ID*>& outIds, const ID& queriedId, int k)
{
	auto& bucket = m_BucketArray.getBucket(bucketIdx);
	if (bucket.size() == 0) return;

	if (bucket.size() < k) {
		for (auto& contact : bucket) {
			outIds.push_back(&contact.id());
		}
		return;
	}

    struct Candidate { int distance;  const ID* Id; };

    std::vector<Candidate> candidates;
    for (auto& contact : bucket) {
        int dist = queriedId.distance(contact.id());
        if (candidates.size() < k+1){
            candidates.emplace_back(Candidate{dist, &contact.m_id});
        }
        else {
            candidates[k].distance = dist;
            candidates[k].Id = &contact.m_id;
        }
        std::sort(candidates.begin(), candidates.end(), [](const auto& a, const auto& b){
            return a.distance < b.distance;
        });
    }
    assert(candidates.size() >= k);
    auto it = candidates.begin();
	while (outIds.size() < k) {
        outIds.push_back(it->Id);
		++it;
	}
}

std::vector<const ID*> Node::findClosestNodes(int k, const ID& senderId, const ID& queriedId)
{
	std::vector<const ID*> res;
	// start with the bucket where ID could be
	int bucketIndex = m_BucketArray.calcBucketIndex(queriedId);
	fill(bucketIndex, res, queriedId, k);

    // not enough ids
	if (res.size() < k) {
        //- MAKES A WORSE RESULT!!!!!!!!!!
//        for(size_t i = bucketIndex - 1; i >= 0; --i) {
//            fill(i, res, queriedId, k);
//        }
//        for(size_t i = bucketIndex + 1; i < TREE_SIZE; ++i) {
//            fill(i, res, queriedId, k);
//        }
        int nextBucketIndex = bucketIndex, prevBucketIndex = bucketIndex;
        size_t i = 1;
        for (; nextBucketIndex < TREE_SIZE && prevBucketIndex >= 0; ++i)
        {
            nextBucketIndex = bucketIndex + i;
            prevBucketIndex = bucketIndex - i;
            if (nextBucketIndex < TREE_SIZE) {
                fill(nextBucketIndex, res, queriedId, k);
            }
            if (prevBucketIndex >= 0) {
                fill(prevBucketIndex, res, queriedId, k);
            }
        }
        for (size_t j = i; res.size() < k && nextBucketIndex < TREE_SIZE; ++j)
        {
            nextBucketIndex = bucketIndex + j;
            if (nextBucketIndex < TREE_SIZE) {
                fill(nextBucketIndex, res, queriedId, k);
            }
        }
        for (size_t j = i; res.size() < k && prevBucketIndex >= 0; ++j)
        {
            prevBucketIndex = bucketIndex - j;
            if (prevBucketIndex >= 0) {
                fill(prevBucketIndex, res, queriedId, k);
            }
        }
    }
    return res;
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
		return findClosestNodes(CLOSEST_NODES, senderId, queriedId);
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
	if (auto it = m_findThisId.find(queriedId); it == m_findThisId.end()) {
		return;
	}
	m_interrogatedNodes.insert(responserId);
	addNode(responserId);
	if (m_findThisId[queriedId] > FIND_NODE_THRESHOLD) {
		onFindNodeEnd(false, queriedId);
		return;
	}
	if (*ids[0] == queriedId) {
		onFindNodeEnd(true, queriedId);
	}
	else {
        int dist = responserId.distance(queriedId);
		for (auto id : ids) {
#ifdef OPT_MODE
			if (auto it = m_interrogatedNodes.find(*id); it == m_interrogatedNodes.end()) {
#endif
//                if(id->distance(queriedId) <= dist) - MAKES A WORSE RESULT!!!!!!!!!!
                        m_protocol.sendFindNode(*id, this->id(), queriedId); // Swarm does this
#ifdef OPT_MODE
            }
#endif
		}
	}

}

void Node::onFindNodeStart(const ID& queriedId)
{
	m_findThisId[queriedId]++;
}

void Node::onFindNodeEnd(bool found, const ID& queriedId)
{
    //m_findThisId.erase(queriedId);
	m_interrogatedNodes.clear();
	if (found) {
        m_eventHandler.onNodeFound(queriedId);
	}
	else {
        m_eventHandler.onNodeNotFound(queriedId);
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



