#pragma once
#include "ID.h"
#include <boost/asio.hpp>

class node
{
private:
    ID                              m_id;
    std::uint16_t                   m_port;
    std::string                     m_address;
    boost::asio::ip::tcp::endpoint  m_endpoint;

public:
    node();
    node(std::string address, uint32_t port)
        : m_port(port), m_address(address) {}

    bool create_endpoint();

    ID get_node_id() const
    {
        return m_id;
    }

    friend bool operator==(const node & l, const node & r)
    {
        return l.m_id == r.m_id;
    }

};