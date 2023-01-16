#pragma once

#include "bucket_list.h"

class contact : public node
{
private:
    bucket_list m_bucket_list;

public:
    contact();

    bool add_node(const node & node);
};

