#pragma once

#include "Contact.h"
#include <set>

class Bucket
{
private:
    std::set<Contact>  m_Bucket;
    static size_t      m_maxBucketSize;
    //TODO: need a reference to my BucketMap to get to my Node
    // (for updateNode: pinging front peers in list)
public:
    Bucket() = default;

    const std::set<Contact>& bucket() const;

    std::set<Contact>::iterator find_node(const ID& id);

    bool addNode(const Contact &contact);
    bool addNode(const ID& id);

    bool containsNode(const Contact& contact);
    bool containsNode(const ID& id);

    size_t removeNode(const Contact& contact);
    size_t removeNode(const ID& id);

    bool isFull() const;
    bool isEmpty() const;
    size_t size() const;
    friend std::ostream& operator<<(std::ostream& out, const Bucket& b);

};
