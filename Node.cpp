#include "Node.h"

const ID & Node::id() const
{
    return m_id;
}

boost::asio::ip::tcp::endpoint Node::getNodeEndpoint() const
{
    assert (m_endpointSet);
    return m_endpoint;
}

NodeInfo Node::nodeInfo() const
{
    return m_info;
}

bool Node::createEndpoint()
{
    boost::system::error_code ec;
    boost::asio::ip::address ip_address =
            boost::asio::ip::address::from_string(m_address, ec);
    if (ec.value() != 0) {
        std::cerr
                << "Failed to parse the IP address. Error code = "
                << ec.value() << ". Message: " << ec.message();
        return false;
    }
    m_endpoint = boost::asio::ip::tcp::endpoint {ip_address, m_port};
    m_endpointSet = true;
    return true;
}

void Node::randomizeId()
{
    m_id.randomize();
}

void Node::addNode(const Node* node)
{
    assert((void("Node shouldn't add itself into its bucketMap"),
            m_id != node->id()));
    m_BucketMap.addNode(node->id(), node);
}

bool operator==(const Node & l, const Node & r)
{
    return l.m_id == r.m_id;
}

//void Node::ping(const ID & queryingId, const ID & id)
//{

//}

ID Node::ping(const ID & id) const
{
    assert((void("Pinging myself is pointless"), m_id != id));
//    addNode(id);
    return m_id;
}

IKademliaTransportProtocol& Node::protocol()
{
    return m_protocol;
}

Node* Node::pickRandomNode(Bucket& b)
{
    auto it = b.bucket().begin();
    std::uniform_int_distribution<int> range(0, b.size());
    uint16_t randomNodeNumber = range(m_randomGenerator);
    std::advance(it, randomNodeNumber);
    return it->second;
}

void Node::populate(Node * bootstrapNode)
{
    /*
     * find the only one non-empty bucket
     * there lies the bootstrap node
     * enter her bucketMap
     * according to bootstrap node's ID, find # of bucket where myID can be lying
     * of course, it is not there -> if this bucket is empty,       search for a close non-empty bucket
     *                                                 not empty,   take <n> random nodes, recursion.
     */
    size_t bucketIndex = m_BucketMap.calcBucketIndex(bootstrapNode->id());
    BucketMap bootstrapBucketMap = bootstrapNode->m_BucketMap;
    std::vector<Bucket> bucketsInBootstrap = bootstrapBucketMap.nonEmptyBuckets();
    // i am NOT the first node in the network, except for bootstrapNode
    for(Bucket &b : bucketsInBootstrap) {
        for(uint16_t i = 0; i < m_spreadNodes || i < b.size(); ++i) {
            populate(pickRandomNode(b));
        }
    };
    bootstrapNode->addNode(this);
}


