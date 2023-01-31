#pragma once

#include "ID.h"
#include <cstdint>
#include <boost/asio/ip/tcp.hpp>

struct Contact
{
    ID                              m_id;
    boost::asio::ip::tcp::endpoint  m_endpoint; // can split into address + port
    Contact(const ID &id) : m_id(id) {};
    ID id() { return m_id; };
    bool operator==(const Contact & otherContact) { return m_id == otherContact.m_id; };
    bool operator!=(const Contact & otherContact) { return m_id != otherContact.m_id; };
    bool operator<(const Contact & otherContact) { return m_id < otherContact.m_id; };
    bool operator>(const Contact & otherContact) { return otherContact.m_id < m_id; };

};

