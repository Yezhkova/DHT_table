#pragma once

//#include "Bucket.h"
#include "Contact.h"
#include <array>
#include <set>
#include <optional>
#include <iostream>

class Node;
using BucketIndex = size_t;
using Bucket = std::set<Contact>;

class BucketArray
{
private:

    Node&                     m_node;
    std::array<Bucket, 160>   m_Buckets;
    static int                g_bucketSize;
    static int                g_treeSize;

public:

    BucketArray() = delete;
    BucketArray(Node& node) : m_node(node) {};

    bool addNode(const ID& id, int BucketIndex);
    bool removeNode(const ID& id);
    const Contact* getContactPtr(const ID& id) const;

    const std::array<Bucket, 160>& data() const;
    //const ID& id() const { return m_node.id(); }
    const Bucket& getBucket(int index) const;
    const size_t size() const;
    const size_t bucketSize(int bucketIdx) const;
    const bool bucketFull(int bucketIdx) const;

    int calcBucketIndex(const ID& id);
    friend std::ostream& operator<< (std::ostream& out, const BucketArray& b);
};

