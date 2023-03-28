#pragma once

#include "Contact.h"
#include "ID.h"
#include "INodeEventHandler.h"
#include "NodeInfo.h"
#include "BucketMap.h"
//#include <boost/asio.hpp>
#include <cassert>
#include "IDhtTransportProtocol.h"
//#include <boost/chrono.hpp>
#include <map>

//using system_clock = boost::chrono::system_clock;

class Node
{
private:
    bool                           m_online = true;
    Contact                       m_contact;
    INodeEventHandler&      m_eventHandler;
    BucketArray                  m_BucketArray;
    IDhtTransportProtocol&   m_protocol;
    ITimer&                       m_timerProtocol;
    uint64_t                       m_index = 0;
    static std::mt19937         m_randomGenerator;

    // TODO: map<const ID*, int>, set<const ID*> - ?????????????????????

    std::map<ID, int>           m_findThisId; // pair<queriedId, packetCounter>
    std::map<ID, int>           m_pingMap;
    std::set<ID>                  m_interrogatedNodes;

public:
    Node(ID id, IDhtTransportProtocol& protocol, ITimer& timer, INodeEventHandler& peer)
        : m_contact(id)
        , m_eventHandler(peer)
        , m_BucketArray(*this)
        , m_protocol(protocol)
        , m_timerProtocol(timer) {};

    const ID& id() const { return m_contact.m_id; }

    const BucketArray& buckets() const { return m_BucketArray; }

    const Contact& contact() const { return m_contact; }

    IDhtTransportProtocol& protocol(){ return m_protocol; }

    INodeEventHandler& eventHandler() { return m_eventHandler; }

    std::map<ID, int>& findThisIdQueries() { return m_findThisId; }

    std::set<ID>& interrogatedNodes() { return m_interrogatedNodes; }

    const uint64_t label() const { return m_index; }

    void setLabel(uint64_t label) { m_index = label; }

    void randomizeId();
    bool addNode(const ID& id);
    bool removeNode(const ID& id);

    const ID& pickRandomNode(const Bucket& bucket) const;
    std::vector<const ID*> findClosestNodes(int k, const ID& senderId, const ID& queriedId);

    const ID& findClosestNode(const ID& senderId, const ID& queriedId) {
        return *(findClosestNodes(1, senderId, queriedId)[0]);
    }

    friend bool operator==(const Node& l, const Node& r);

    bool receivePing(const ID& requestorId);

    void receivePingResponse(bool online
        , const ID& queriedId);

    std::vector<const ID*> receiveFindNode(const ID& senderId
        , const ID& queriedId);

    void receiveFindNodeResponse(const ID& queriedId
        , const std::vector<const ID*>& ids
        , const ID& responserId);

    void onFindNodeStart(const ID& queriedId);
    void onFindNodeEnd(bool found, const ID& queriedId);
    void onPingStart(const ID& queriedId);
    void onPingEnd(bool online, const ID& queriedId);

    void setOffline() { m_online = false; }

    void setOnline() { m_online = true; }

    bool online() { return m_online; };

private:

    void fill(int bucketIdx, std::vector<const ID*>& outIds, const ID& senderId, int k);
};

