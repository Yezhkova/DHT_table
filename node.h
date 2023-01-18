#pragma once
#include "ID.h"
#include "nodeinfo.h"
#include "bucket_map.h"
#include <boost/asio.hpp>

class Bucket;
class Node
{
private:
    ID                              m_id;
    std::uint16_t                   m_port;
    std::string                     m_address;
    boost::asio::ip::tcp::endpoint  m_endpoint;
    bool                            m_endpoint_set = false;
    BucketMap                       m_bucket_map;
    NodeInfo                        m_info;

public:
    Node();
    Node(std::string address, uint32_t port);

    void create_endpoint();

    const ID & get_node_id() const;
    boost::asio::ip::tcp::endpoint get_node_endpoint() const;
    NodeInfo get_node_info() const;

    void ping(const ID & id);

    friend bool operator==(const Node & l, const Node & r);

};
