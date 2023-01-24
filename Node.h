#pragma once
#include "ID.h"
#include "NodeInfo.h"
#include "BucketMap.h"
#include <boost/asio.hpp>
#include <cassert>
#include "IKademliaTransportProtocol.h"
class Swarm;
class Node
{
private:
    ID                                  m_id;
    std::uint16_t                       m_port;
    std::string                         m_address;
    boost::asio::ip::tcp::endpoint      m_endpoint;
    bool                                m_endpointSet = false;
    BucketMap                           m_BucketMap;
    NodeInfo                            m_info;
    size_t                              m_treeSize = 160;
    IKademliaTransportProtocol&         m_protocol;

public:
    Node() = delete;
    Node(Node &&) = default; // move constructor
//    Node(std::string address, uint32_t port);
    Node(ID id, IKademliaTransportProtocol& protocol) : m_id(id), m_protocol(protocol) { }

    void randomizeId();
    bool createEndpoint();

    const ID & getNodeId() const;
    boost::asio::ip::tcp::endpoint getNodeEndpoint() const;
    NodeInfo getNodeInfo() const;

    void ping(const ID & id);
    void ping(const ID & queryingId, const ID & id);

    void addNode(const Node & newNode);

    friend bool operator==(const Node & l, const Node & r);
    IKademliaTransportProtocol& protocol();

};
