#pragma once
//#include "Contact.h"
//#include "ID.h"
//#include "NodeInfo.h"
//#include "BucketMap.h"
//#include <boost/asio.hpp>
//#include <cassert>
//#include "IKademliaTransportProtocol.h"
//#include "Utils.h"
//#include <boost/chrono.hpp>

//class Node
//{
//private:
//    bool                                m_isStarting = true;
//    Contact                             m_contact;
//    Peer*                               m_peer = nullptr;
//    BucketMap                           m_BucketMap;
//    size_t                              m_treeSize = 160;
//    IKademliaTransportProtocol&         m_protocol;
//    NodeInfo                            m_info; // last seen time
//    std::mt19937                        m_randomGenerator;

//public:
//    Node() = delete;
//    Node(Node &&) = default;                                // move constructor
//    Node(ID id, IKademliaTransportProtocol& protocol, Peer* peer)
//        : m_contact(id)
//        , m_protocol(protocol)
//        , m_info(boost::chrono::system_clock::now())
//        , m_peer(peer) {};

//    ID id();
//    const BucketMap& bucketMap();
//    IKademliaTransportProtocol& protocol();
//    NodeInfo nodeInfo();

//    void randomizeId();
//    void addNode(const ID& id);
//    void updateNode(const ID& id);
//    ID pickRandomNode(Bucket& b);
//    std::vector<Node> findClosestNodes(uint16_t k, const ID & id);

//    friend bool operator==(const Node & l, const Node & r);

//    void sendPing(const ID & recipientId, const ID & senderId, const ID & queriedId);
//    void sendPingResponse(const ID & queryingId, const ID & queriedId);
//    void receivePing(const ID & queryingId, const ID & queriedId);
//    void receivePingResponse(const ID & queryingId, const ID & queriedId);

//    void sendFindNode(const ID & senderId, const ID & queriedId);
//    void sendFindNodeResponse(const ID & recipientId, const ID & senderId, const ID & queriedId);
//    void receiveFindNode(const ID & myID, const ID & senderId, const ID & queriedId);
//    void receiveFindNodeResponse(const ID & myId, const ID & queriedId);
//};
