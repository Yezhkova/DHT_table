#include "Node.h"

ID Node::id() {
    return m_contact.id();
}

void Node::randomizeId()
{
    m_contact.id().randomize();
}

void Node::addNode(const Contact& contact)
{
    assert((void("Node shouldn't add itself into its bucketMap"),
            m_contact != contact));
    m_BucketMap.addNode(contact);
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

void Node::sendFindNode(const ID & recipientId, const ID & senderId, const ID & queriedId)
{


}

void Node::receiveFindNode(const ID & myID, const ID & senderId, const ID & queriedId)
{
    if(m_BucketMap.containsNode(queriedId))
    {
        sendFindNodeResponse(senderId, myID, queriedId);
    } else {

    }

}


