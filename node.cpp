#include "node.h"

bool node::create_endpoint()
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
    return true;
}

