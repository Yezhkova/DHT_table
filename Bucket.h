#pragma once

#include "Contact.h"
#include <list>

class Bucket
{
private:
    std::list<Contact>  m_Bucket;             // map<ID, boost::asio::tcp::endpoint> if TCP
    static size_t       m_maxBucketSize;
    //TODO: need a reference to my BucketMap to get to my Node
    // (for updateNode: pinging front peers in list)
public:
    Bucket() = default;

    const std::list<Contact>& bucket() const;

    std::list<Contact>::iterator find_node(const ID& id);

    bool updateNode(const Contact &contact);
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
