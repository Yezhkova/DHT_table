#pragma once

#include "Contact.h"
#include <set>

class Bucket
{
private:
    std::set<Contact>   m_Bucket;             // map<ID, boost::asio::tcp::endpoint> if TCP
    static size_t       m_maxBucketSize;

public:
    Bucket() = default;

    const std::set<Contact>& bucket() const;

    auto find_node(const ID& id);

    bool addNode(const Contact& contact);
    bool addNode(const ID& id);

    bool containsNode(const Contact& contact);
    bool containsNode(const ID& id);

    bool removeNode(const Contact& contact);
    bool removeNode(const ID& id);

    bool isFull() const;
    bool isEmpty() const;
    size_t size() const;
    friend std::ostream& operator<<(std::ostream& out, const Bucket& b);

};
