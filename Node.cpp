#include "Swarm.h"
#include "Utils.h"

size_t Node::m_treeSize = 160;
std::mt19937 Node::m_randomGenerator;

ID Node::id() {
    return m_contact.id();
}

const BucketMap& Node::bucketMap() {
    return m_BucketMap;
}

NodeInfo Node::nodeInfo() {
    return m_info;
}

std::weak_ptr<Peer> Node::peer()
{
    return m_peer;
}

void Node::randomizeId()
{
    m_contact.randomize();
}

void Node::addNode(const ID& id)
{
    assert((void("Node shouldn't add itself into its bucketMap"),
            m_contact.id() != id));
    m_BucketMap.addNode(id);
}

void Node::updateNode(const ID& id)
{
    if(std::shared_ptr<Peer> sptPeer = m_peer.lock())
        {
            sptPeer->swarm().lock()->getPeer(id)->node().nodeInfo().
                    updateLastSeen(boost::chrono::system_clock::now());
        }
        else LOG("updateNode error: cannot lock Peer weak_ptr");
}

bool operator==(const Node& l, const Node& r)
{
    return l.m_contact.m_id == r.m_contact.m_id;
}

IKademliaTransportProtocol& Node::protocol()
{
    return m_protocol;
}


const ID& Node::pickRandomNode(const Bucket& b) const
{
    auto it = b.bucket().begin();
    std::uniform_int_distribution<int> range(0, b.size()-1);
//    std::mt19937 m_randomGenerator;
    int randomNodeNumber = range(m_randomGenerator);
    std::advance(it, randomNodeNumber);
    return it->id();
}

void Node::fill(uint16_t bucketIndex, std::vector<ID>& ids, uint16_t k)
{
    for(auto& contact: m_BucketMap.getNodesAtDepth(bucketIndex).bucket())
    {
        if(ids.size() < k)
        {
            ids.push_back(contact.id());
        }
    }
}

std::vector<ID> Node::findClosestNodes(uint16_t k, const ID & id)
{
    std::vector<ID> res;
    uint16_t bucketIndex = m_BucketMap.calcBucketIndex(id);
    uint16_t actualBucketSize = m_BucketMap.getNodesAtDepth(bucketIndex).size();
    if(actualBucketSize < k)
    {
        int nextBucket = bucketIndex, prevBucket = bucketIndex;
        for(size_t i = 1; res.size() < k && nextBucket < m_treeSize && prevBucket > -1; ++i)
        {
            nextBucket = bucketIndex + i;
            prevBucket = bucketIndex - i;
            fill(nextBucket, res, k);
            fill(prevBucket, res, k);
        }
    }
    else
    {
        for(size_t i = 0; i < k; ++i)
        {
            res.push_back(pickRandomNode(m_BucketMap
                                         .getNodesAtDepth(bucketIndex)));
        }
    }
    return res;
}


void Node::sendFindNode(const ID & senderId, const ID & queriedId)
{
    receiveFindNode(m_contact.id(), senderId, queriedId);
}

void Node::receiveFindNode(const ID & myID,
                           const ID & senderId, const ID & queriedId)
{
    if(m_BucketMap.containsNode(queriedId))
    {
        sendFindNodeResponse(senderId, myID, queriedId);
        addNode(senderId);
        updateNode(queriedId);
    }
    else
    {
        std::vector<ID> closestNodes = findClosestNodes(3, queriedId);
        for(auto& id : closestNodes)
        {
                if(std::shared_ptr<Peer> sptPeer = m_peer.lock())
                {
                    sptPeer->swarm().lock()->getPeer(id)->node()
                            .sendFindNode(senderId, queriedId);
                    sptPeer->swarm().lock()->getPeer(id)->node()
                            .addNode(senderId);
                }
                else LOG("receiveFindNode error: cannot lock Swarm weak_ptr");
        }
    }
}

void Node::sendFindNodeResponse(const ID & recipientId,
                                const ID & myId, const ID & queriedId)
{
    if(std::shared_ptr<Peer> sptPeer = m_peer.lock())
        {
            sptPeer->swarm().lock()->getPeer(recipientId)->
                    receiveFindNodeResponse(myId, queriedId);
        }
        else LOG("sendFindNodeResponse error: cannot lock Swarm weak_ptr");
}


