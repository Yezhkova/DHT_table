#pragma once

#include "bucket_list.h"

class contact
{
private:
    node        m_node;
    bucket_list m_bucket_list;

public:
    contact();
    contact(const node & node) : m_node(node) {};

    bool add_node(const node & new_node);
};

