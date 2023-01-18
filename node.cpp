#include "node.h"

Node::Node(std::string address, uint32_t port)
    : m_port(port), m_address(address) {}

const ID & Node::get_node_id() const
{
    return m_id;
}

boost::asio::ip::tcp::endpoint Node::get_node_endpoint() const
{
    return m_endpoint;
}

NodeInfo Node::get_node_info() const
{
    return m_info;
}

void Node::create_endpoint()
{
    boost::system::error_code ec;
    boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(m_address, ec);
    if (ec.value() != 0) {
        std::cerr
                << "Failed to parse the IP address. Error code = "
                << ec.value() << ". Message: " << ec.message();
        m_endpoint = boost::asio::ip::tcp::endpoint {boost::asio::ip::address::from_string("0.0.0.0"), 0};
    }
    m_endpoint = boost::asio::ip::tcp::endpoint {ip_address, m_port};
    m_endpoint_set = true;
}

bool operator==(const Node & l, const Node & r)
{
    return l.m_id == r.m_id;
}
