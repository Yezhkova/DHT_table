#pragma once

#include "bucket.h"
#include <list>

class bucket_array
{
private:
    std::array<bucket, 160> m_buckets;

public:
    bucket_array();
    bool add_node(const node & new_node, ID & id);
};
