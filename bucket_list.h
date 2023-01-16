#pragma once

#include "bucket.h"
#include <list>

class bucket_list
{
private:
    std::list<bucket> m_list;
    uint16_t          size = 160;

public:
    bucket_list();
    bool add_node(const node & new_node, const ID & id);
};
