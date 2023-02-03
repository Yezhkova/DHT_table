#pragma once

#include "BucketMap.h"
#include "Contact.h"
#include "ID.h"
#include "IKademliaTransportProtocol.h"
#include "NodeInfo.h"
#include <map>
#include <memory>
#include <random>

class Peer;

class Swarm : public IKademliaTransportProtocol, public std::enable_shared_from_this<Swarm>
{
private:
    std::map<ID, std::shared_ptr<Peer>>  m_peers;
    std::shared_ptr<Peer>                m_bootstrapNode;
    bool                                 m_useTcp = false;

    Swarm() {};

public:

    static Swarm& getInstace(); //

    void init(bool mode, int PeerNumber);
    void generateSwarm(size_t Peers, bool mode);
    void createBootstrapNode();
    bool tcp();
    std::shared_ptr<Peer> bootstrapNode();
    std::map<ID, std::shared_ptr<Peer>> peers() const;
    std::shared_ptr<Peer> getPeer(const ID& id);

    virtual void sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId) override;
    virtual void sendPingResponse(const ID & queryingId, const ID & queriedId) override;

    virtual void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId) override;
    virtual void sendFindNodeResponse(const ID & myId, const ID & queriedId) override;

};

class Node
{
private:
    bool                              m_isStarting = true;
    Contact                           m_contact;
    Peer&                             m_peer;
    BucketMap                         m_BucketMap;
    static size_t                     m_treeSize;
    IKademliaTransportProtocol&       m_protocol;
    NodeInfo                          m_info; // last seen time
    static std::mt19937               m_randomGenerator;

    void fill(uint16_t bucketIndex, std::vector<ID>& ids, uint16_t k);


public:
//    Node() = delete;
    Node(ID id, IKademliaTransportProtocol& protocol, Peer& peer)
        : m_contact(id)
        , m_protocol(protocol)
        , m_peer(peer)
        , m_info(boost::chrono::system_clock::now()) {};

    ID id();
    const BucketMap& bucketMap();
    IKademliaTransportProtocol& protocol();
    NodeInfo nodeInfo();
    std::weak_ptr<Peer> peer();

    void randomizeId();
    void addNode(const ID& id);
    void updateNode(const ID& id);
    const ID& pickRandomNode(const Bucket& b) const;
    std::vector<ID> findClosestNodes(uint16_t k, const ID& id);

    friend bool operator==(const Node & l, const Node & r);

    void sendPing(const ID & recipientId, const ID & senderId, const ID & queriedId);
    void sendPingResponse(const ID & queryingId, const ID & queriedId);
    void receivePing(const ID & queryingId, const ID & queriedId);
    void receivePingResponse(const ID & queryingId, const ID & queriedId);

    void sendFindNode(const ID & senderId, const ID & queriedId);
    void sendFindNodeResponse(const ID & recipientId, const ID & senderId, const ID & queriedId);
    void receiveFindNode(const ID & myID, const ID & senderId, const ID & queriedId);
    void receiveFindNodeResponse(const ID & myId, const ID & queriedId);

};

class Peer: public std::enable_shared_from_this<Peer>
{
private:
    Node                  m_node;

public:
    Peer() = delete;
    Peer(const Peer&) = default;
    Peer(Peer && ) = default;
    Peer(ID id,
         IKademliaTransportProtocol& protocol,
         bool useTcp)
        : m_node(id, protocol, *this) {};

    ID id();
    Node & node();
    void randomize();

    void start(const ID & bootstrapId);

    void addNode(const ID & id);

    void sendPing(const ID & recipientId, const ID & queryingId, const ID & queriedId);
    void sendPingResponse(const ID & queryingId, const ID & queriedId);

    void receivePing(const ID & queryingId, const ID & queriedId);
    void receivePingResponse(const ID & queryingId, const ID & queriedId);

    void sendFindNode(const ID & recipientId, const ID & myId, const ID & queriedId);
    void sendFindNodeResponse(const ID & myId, const ID & queriedId);

    void receiveFindNode(const ID & recipientId, const ID & myId, const ID & queriedId);
    void receiveFindNodeResponse(const ID & senderId, const ID & queriedId);

};

