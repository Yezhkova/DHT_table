#pragma once

#include "Contact.h"
#include "ID.h"
#include "INodeEventHandler.h"
#include "NodeInfo.h"
#include "BucketMap.h"
#include <boost/asio.hpp>
#include <cassert>
#include "IKademliaTransportProtocol.h"
#include <boost/chrono.hpp>

using system_clock = boost::chrono::system_clock;

class Node
{
private:
    bool                              m_isStarting = true;
    Contact                           m_contact;
    INodeEventHandler&                m_eventHandler;
    BucketMap                         m_BucketMap;
    static size_t                     m_treeSize;
    IKademliaTransportProtocol&       m_protocol;
    NodeInfo                          m_info; // last seen time
    static std::mt19937               m_randomGenerator;

    void fill(std::optional<Bucket>& bucket, std::vector<ID>& ids, uint16_t k);


public:
    Node(ID id, IKademliaTransportProtocol& protocol, INodeEventHandler& peer)
        : m_contact(id)
        , m_eventHandler(peer)
        , m_BucketMap(*this)
        , m_protocol(protocol)
        , m_info(boost::chrono::system_clock::now()) {};

    const ID& id() const;
    const BucketMap& bucketMap() const;
    const Contact& contact() const;
    IKademliaTransportProtocol& protocol();
    NodeInfo& nodeInfo();
    INodeEventHandler& eventHandler();

    void randomizeId();
    bool addNode(const ID& id);
    void updateLastSeen(const ID& id
                        , boost::chrono::system_clock::time_point time);
    const ID& pickRandomNode(const Bucket& b) const;
    std::vector<ID> findClosestNodes(uint16_t k, const ID& id);

    friend bool operator==(const Node & l, const Node & r);

    void sendPing(const ID & queryingId);
    void sendPingResponse(const ID & queryingId);
    void receivePing(const ID & queryingId, const ID & queriedId);
    void receivePingResponse(const ID & queryingId, const ID & queriedId);

    void sendFindNode(const ID & senderId, const ID & queriedId);
    void sendFindNodeResponse(const ID & recipientId, const ID & senderId, const ID & queriedId);
    std::vector<ID> receiveFindNode(const ID& senderId
                                    , const ID& queriedId
                                    , system_clock::time_point time);

    void receiveFindNodeResponse(
        const ID& queriedId
        , std::vector<ID> ids
        , const ID& responserId);

};

