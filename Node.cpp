#include "Node.h"

const ID & Node::id() const
{
    return m_id;
}

boost::asio::ip::tcp::endpoint Node::getNodeEndpoint() const
{
    assert (m_endpointSet);
    return m_endpoint;
}

NodeInfo Node::nodeInfo() const
{
    return m_info;
}

bool Node::createEndpoint()
{
    boost::system::error_code ec;
    boost::asio::ip::address ip_address =
            boost::asio::ip::address::from_string(m_address, ec);
    if (ec.value() != 0) {
        std::cerr
                << "Failed to parse the IP address. Error code = "
                << ec.value() << ". Message: " << ec.message();
        return false;
    }
    m_endpoint = boost::asio::ip::tcp::endpoint {ip_address, m_port};
    m_endpointSet = true;
    return true;
}

void Node::randomizeId()
{
    m_id.randomize();
}

void Node::addNode(const ID & id)
{
    assert((void("Node shouldn't add itself into its bucketMap"),
            m_id != id));
    m_BucketMap.addNode(id);
}

bool operator==(const Node & l, const Node & r)
{
    return l.m_id == r.m_id;
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

void Node::receiveFindNode(const ID & recipientId, const ID & senderId, const ID & queriedId)
{

}


