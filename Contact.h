#pragma once

#include "ID.h"
#include <cstdint>
#include <boost/asio/ip/tcp.hpp>

struct Contact
{
    ID                              m_id;
    boost::asio::ip::tcp::endpoint  m_endpoint; // can split into address + port
public:
    Contact(const ID &id) : m_id(id) {};
};

