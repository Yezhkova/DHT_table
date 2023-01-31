#pragma once

#include "Contact.h"
#include <set>

class Bucket
{
private:
    std::set<Contact>   m_Bucket;             // map<ID, boost::asio::tcp::endpoint> if TCP
    size_t              m_maxBucketSize = 20;

public:
    Bucket() = default;

    std::set<Contact> bucket() { return m_Bucket; };
    std::set<Contact>::iterator find_node(const Contact &contact);
    bool addNode(const Contact &contact);
    bool containsNode(const Contact &contact);
    bool removeNode(const Contact &contact);
    bool isFull();
    size_t size();

};
