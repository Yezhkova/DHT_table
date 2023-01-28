#pragma once

#include "ID.h"
#include <set>

class Bucket
{
private:
    std::set<ID>    m_Bucket;             // map<ID, boost::asio::tcp::endpoint> if TCP
    size_t          m_maxBucketSize = 20;

public:
    Bucket() = default;

    std::set<ID> bucket() { return m_Bucket; };
    std::set<ID>::iterator find_node(const ID &id);
    bool addNode(const ID &id);
    bool containsNode(const ID &id);
    bool removeNode(const ID &id);
    bool isFull();
    size_t size();

};
