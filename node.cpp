#include "node.h"

Node::Node(std::string address, uint32_t port)
    : m_address(address)
    , m_port(port)
{
    randomize_id();
    m_bucket_map = BucketMap(m_id);
}

Node::Node(std::string address, uint32_t port, std::weak_ptr<Swarm> swarm)
{
    // std::weak_ptr<Swarm> swarm goes to m_protocol;
}


const ID & Node::get_node_id() const
{
    return m_id;
}

boost::asio::ip::tcp::endpoint Node::get_node_endpoint() const
{
    assert (m_endpoint_set);
    return m_endpoint;
}

NodeInfo Node::get_node_info() const
{
    return m_info;
}

bool Node::create_endpoint()
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
    m_endpoint_set = true;
    return true;
}

void Node::randomize_id()
{
    m_id.randomize();
}

void Node::add_node(const Node & new_node)
{
    m_bucket_map.addNode(new_node.get_node_id(), new_node.get_node_info());
}

bool operator==(const Node & l, const Node & r)
{
    return l.m_id == r.m_id;
}
