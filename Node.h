#pragma once
#include "ID.h"
#include "NodeInfo.h"
#include "BucketMap.h"
#include <boost/asio.hpp>
#include <cassert>
#include "IKademliaTransportProtocol.h"
#include "Utils.h"
#include <boost/chrono.hpp>
class Swarm;
class Node
{
private:
    bool                                m_isStarting = true;
    std::uint16_t                       m_port;
    std::string                         m_address;
    boost::asio::ip::tcp::endpoint      m_endpoint;
    bool                                m_endpointSet = false;
    BucketMap                           m_BucketMap;
    size_t                              m_treeSize = 160;
    IKademliaTransportProtocol&         m_protocol;

    ID                                  m_id;
    NodeInfo                            m_info;

    std::mt19937                        m_randomGenerator;
    const int                           m_spreadNodes = 3;

public:
    Node() = delete;
    Node(Node &&) = default;                                // move constructor
    Node(ID id, IKademliaTransportProtocol& protocol)
        : m_id(id), m_protocol(protocol)
        , m_info(boost::chrono::system_clock::now()) { };

    void randomizeId();
    bool createEndpoint();

    const ID & id() const;
    boost::asio::ip::tcp::endpoint getNodeEndpoint() const;
    NodeInfo nodeInfo() const;
    BucketMap bucketMap() { return m_BucketMap;};

    void addNode(const ID & id);

    friend bool operator==(const Node & l, const Node & r);
    IKademliaTransportProtocol& protocol();
    ID pickRandomNode(Bucket& b);

    void sendPing(const ID & recipientId, const ID & senderId, const ID & queriedId);
    void sendPingResponse(const ID & queryingId, const ID & queriedId);
    void receivePing(const ID & queryingId, const ID & queriedId);
    void receivePingResponse(const ID & queryingId, const ID & queriedId);

    void sendFindNode(const ID & recipientId, const ID & senderId, const ID & queriedId);
    void sendFindNodeResponse(const ID & myId, const ID & queriedId);
    void receiveFindNode(const ID & recipientId, const ID & senderId, const ID & queriedId);
    void receiveFindNodeResponse(const ID & myId, const ID & queriedId);
};
