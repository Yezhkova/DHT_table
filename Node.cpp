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

NodeInfo Node::getNodeInfo() const
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

void Node::addNode(const Node & newNode)
{
    m_BucketMap.addNode(newNode.id(), newNode.getNodeInfo());
}

bool operator==(const Node & l, const Node & r)
{
    return l.m_id == r.m_id;
}

//void Node::ping(const ID & queryingId, const ID & id)
//{

//}
void Node::ping(const ID & id) const
{
    LOG(std::string(id) << '\n');
}

IKademliaTransportProtocol& Node::protocol()
{
    return m_protocol;
}


