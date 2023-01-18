#pragma once

#include "bucket.h"
#include <list>

class BucketArray
{
private:
    std::array<bucket, 160> m_buckets;

public:
    bucket_array();
    bool add_node(const node & new_node, const ID & id);
};

// camelStyle = me, snake_style = STL
