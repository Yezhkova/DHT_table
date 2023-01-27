#pragma once

#include "BucketMap.h"
#include "IKademliaTransportProtocol.h"
#include <boost/chrono.hpp>
#include <boost/asio.hpp>

class NodeInfo
{
private:
    boost::chrono::system_clock::time_point m_lastSeen;
//    std::uint16_t                           m_port;
//    std::string                             m_address;
//    boost::asio::ip::tcp::endpoint          m_endpoint;
//    bool                                    m_endpointSet = false;
//    BucketMap                               m_BucketMap;
//    size_t                                  m_treeSize = 160;
//    IKademliaTransportProtocol&             m_protocol;

public:
    NodeInfo() = delete;
//    NodeInfo(IKademliaTransportProtocol& protocol,
//             boost::chrono::system_clock::time_point time)
//        : m_lastSeen(time)
//        , m_protocol(protocol) {};

    NodeInfo(boost::chrono::system_clock::time_point time)
        : m_lastSeen(time) {};

//    boost::chrono::system_clock::time_point lastSeen() { return m_lastSeen; };
//    std::uint16_t port() { return m_port; };
//    std::string address() { return m_address; };
//    boost::asio::ip::tcp::endpoint endpoint() { return m_endpoint; };
//    bool endpointSet() { return m_endpointSet; };
//    BucketMap bucketMap() { return m_BucketMap; };
//    IKademliaTransportProtocol& protocol() { return m_protocol; };
};

