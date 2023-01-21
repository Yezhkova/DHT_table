#pragma once
#include "ID.h"
#include "NodeInfo.h"
#include "BucketMap.h"
#include <boost/asio.hpp>
#include <cassert>
//#include "Swarm.h"
#include "IKademliaTransportProtocol.h"
class Swarm;
class Node
{
protected:
    ID                              m_id;
    std::uint16_t                   m_port;
    std::string                     m_address;
    boost::asio::ip::tcp::endpoint  m_endpoint;
    bool                            m_endpointSet = false;
    BucketMap                       m_BucketMap;
    NodeInfo                        m_info;
    size_t                          m_treeSize = 160;
    std::unique_ptr<IKademliaTransportProtocol> m_protocol;

public:
    Node() = default;
    Node(std::string address, uint32_t port);
    Node(std::string address, uint32_t port, std::weak_ptr<Swarm> Swarm);
    Node(const Node& n);


    void randomizeId();
    bool createEndpoint();

    const ID & getNodeId() const;
    boost::asio::ip::tcp::endpoint getNodeEndpoint() const;
    NodeInfo getNodeInfo() const;

    void ping(const ID & id);
    void addNode(const Node & newNode);

    friend bool operator==(const Node & l, const Node & r);

};
