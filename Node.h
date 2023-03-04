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
    bool                              m_online = true;
    Contact                           m_contact;
    INodeEventHandler&                m_eventHandler;
    BucketMap                         m_BucketMap;
    static size_t                     m_treeSize;
    IKademliaTransportProtocol&       m_protocol;
    NodeInfo                          m_info; // last seen time
    static std::mt19937               m_randomGenerator;
    std::map<ID, uint32_t>            m_findNodeMap; // pair<queriedId, packetCounter>
    std::map<ID, uint32_t>            m_pingMap;

    void fill(std::optional<Bucket>& bucket, std::vector<ID>& ids, int k);

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
    bool removeNode(const ID& id);
    void updateLastSeen(const ID& id
                        , boost::chrono::system_clock::time_point time);

    const ID& pickRandomNode(const Bucket& b) const;
    std::vector<ID> findClosestNodes(int k, const ID& id);

    friend bool operator==(const Node & l, const Node & r);

    bool receivePing(const ID & requestorId);

    void receivePingResponse(bool online
                             , const ID & queriedId);

    std::vector<ID> receiveFindNode(const ID& senderId
                                    , const ID& queriedId);

    void receiveFindNodeResponse(const ID& queriedId
                                 , std::vector<ID> ids
                                 , const ID& responserId);

    void onFindNodeStart(const ID& queriedId);
    void onFindNodeEnd(bool found, const ID& queriedId);
    void onPingStart(const ID& queriedId);
    void onPingEnd(bool online, const ID& queriedId);

    void setOffline();
    void setOnline();
    bool online() { return m_online; };

};

