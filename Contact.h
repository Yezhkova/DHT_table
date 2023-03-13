#pragma once

#include "ID.h"
#include <cstdint>
#include <boost/asio/ip/tcp.hpp>

using endpoint = boost::asio::ip::tcp::endpoint;

struct Contact
{
    ID        m_id;
    endpoint  m_endpoint; // can split into address + port

    Contact(const ID &id) : m_id(id) {};
    const ID& id() const {
        return m_id;
    }

    void randomize();
    bool operator == (const Contact & otherContact) const;
    bool operator != (const Contact & otherContact) const;
    bool operator <  (const Contact & otherContact) const;
    bool operator <= (const Contact & otherContact) const;
    bool operator >  (const Contact & otherContact) const;
    bool operator >= (const Contact & otherContact) const;
    friend std::ostream& operator<<(std::ostream& out, const Contact& c);

};

