#pragma once
#include "node.h"
#include <forward_list>

class bucket
{
private:
    std::forward_list<node> m_bucket;
    uint16_t                m_depth = 0;
    uint16_t                m_max_bucket_size = 20;
    bool                    m_empty = true;

public:
    bucket();
    bucket(uint16_t depth) : m_depth(depth) {};
    bucket(uint16_t depth, uint16_t size) : m_depth(depth),
                                            m_max_bucket_size(size) {};

    std::_Fwd_list_iterator<node> find_node(node node);
    bool add_node(const node & new_node);
    bool contains_node(const node & node);
    bool remove_node(const node & node);
    bool is_full();
    uint16_t size();

};
