#include "Node.h"
#include "Swarm.h"
ID Node::id() {
    return m_contact.id();
}

const BucketMap& Node::bucketMap() {
    return m_BucketMap;
}

NodeInfo Node::nodeInfo() {
    return m_info;
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
    m_peer->swarm()->getPeer(id)->node().nodeInfo().
            updateLastSeen(boost::chrono::system_clock::now());
}

bool operator==(const Node& l, const Node& r)
{
    return l.m_contact.m_id == r.m_contact.m_id;
}

IKademliaTransportProtocol& Node::protocol()
{
    return m_protocol;
}

/*
ID Node::pickRandomNode(Bucket& b)
{
    auto it = b.bucket().begin();
    std::uniform_int_distribution<int> range(0, b.size()-1);
    uint16_t randomNodeNumber = range(m_randomGenerator);
    std::advance(it, randomNodeNumber);
    return it->second;
}
*/

std::vector<Node> Node::findClosestNodes(uint16_t k, const ID & id)
{
    std::vector<Node> res;
    return res;
}


void Node::sendFindNode(const ID & senderId, const ID & queriedId)
{
    receiveFindNode(m_contact.id(), senderId, queriedId);
}

void Node::receiveFindNode(const ID & myID, const ID & senderId, const ID & queriedId)
{
    if(m_BucketMap.containsNode(queriedId))
    {
        sendFindNodeResponse(senderId, myID, queriedId);
        addNode(senderId);
        updateNode(queriedId);
    }
    else
    {
        std::vector<Node> closestNodes = findClosestNodes(3, queriedId);
        for(auto & node : closestNodes)
        {
            node.sendFindNode(senderId, queriedId);
            node.addNode(senderId);
        }
    }
}

void Node::sendFindNodeResponse(const ID & recipientId, const ID & myId, const ID & queriedId)
{
    m_peer->swarm()->getPeer(recipientId)->receiveFindNodeResponse(myId, queriedId);
}


