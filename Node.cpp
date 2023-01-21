#include "Node.h"

Node::Node(std::string address, uint32_t port)
    : m_address(address)
    , m_port(port)
{
    randomizeId();
    m_BucketMap = BucketMap(m_id);
}

Node::Node(std::string address, uint32_t port, std::weak_ptr<Swarm> Swarm)
{
    // std::weak_ptr<Swarm> Swarm goes to m_protocol;
}

Node::Node(const Node& n)
    : m_id(n.m_id)
    , m_port(n.m_port)
    , m_address(n.m_address)
    , m_endpoint(n.m_endpoint)
    , m_BucketMap(n.m_BucketMap)
    , m_info(n.m_info) { }


const ID & Node::getNodeId() const
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
    boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(m_address, ec);
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
    m_BucketMap.addNode(newNode.getNodeId(), newNode.getNodeInfo());
}

bool operator==(const Node & l, const Node & r)
{
    return l.m_id == r.m_id;
}

void Node::ping(const ID & id)
{

}

