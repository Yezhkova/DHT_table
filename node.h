#pragma once
#include "ID.h"
#include "nodeinfo.h"
#include "bucket_map.h"
#include <boost/asio.hpp>
#include <cassert>
#include "ikademliatransportprotocol.h"
#include "swarm.h"

class Node
{
protected:
    ID                              m_id;
    std::uint16_t                   m_port;
    std::string                     m_address;
    boost::asio::ip::tcp::endpoint  m_endpoint;
    bool                            m_endpoint_set = false;
    BucketMap                       m_bucket_map;
    NodeInfo                        m_info;
    size_t                          m_tree_size = 160;
    std::unique_ptr<IKademliaTransportProtocol> m_protocol;

public:
    Node() = default;
    Node(std::string address, uint32_t port);
    Node(std::string address, uint32_t port, std::weak_ptr<Swarm> swarm);


    void randomize_id();
    bool create_endpoint();

    const ID & get_node_id() const;
    boost::asio::ip::tcp::endpoint get_node_endpoint() const;
    NodeInfo get_node_info() const;

    void ping(const ID & id);
    void add_node(const Node & new_node);

    friend bool operator==(const Node & l, const Node & r);

};
