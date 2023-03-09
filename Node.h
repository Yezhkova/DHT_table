#pragma once

#include "Contact.h"
#include "ID.h"
#include "INodeEventHandler.h"
#include "NodeInfo.h"
#include "BucketMap.h"
#include <boost/asio.hpp>
#include <cassert>
#include "IDhtTransportProtocol.h"
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
    IDhtTransportProtocol&       m_protocol;
    ITimer&                           m_timerProtocol;
    uint64_t                          m_index;
    NodeInfo                          m_info; // last seen time
    static std::mt19937               m_randomGenerator;
    std::map<ID, int>            m_findNodeMap; // pair<queriedId, packetCounter>
    std::map<ID, int>            m_pingMap;

public:
    Node(ID id, IDhtTransportProtocol& protocol, ITimer& timer, INodeEventHandler& peer)
        : m_contact(id)
        , m_eventHandler(peer)
        , m_BucketMap(*this)
        , m_protocol(protocol)
        , m_timerProtocol(timer)
        , m_info(boost::chrono::system_clock::now()) {};

    const ID& id() const;
    const BucketMap& bucketMap() const;
    const Contact& contact() const;
    IDhtTransportProtocol& protocol();
    NodeInfo& nodeInfo();
    INodeEventHandler& eventHandler();
    uint64_t label() const;
    void setLabel(uint64_t label);

    void randomizeId();
    bool addNode(const ID& id);
    bool removeNode(const ID& id);
    void updateLastSeen(const ID& id
                        , boost::chrono::system_clock::time_point time);

    const ID pickRandomNode(const std::set<Contact>& s) const;
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

private:

    void fill(int idx, std::vector<ID>& ids, int k, const ID& id);

};

