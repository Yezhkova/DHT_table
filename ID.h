#pragma once
#include <functional>
#include <iostream>
#include <sstream>
#include <bitset>
#include <boost/uuid/detail/sha1.hpp>
#include <random>

#define DIGEST 160
#define BITS 4 // 4 bits in hexadecimal character
#define PART 40
//#define MAX 1099511627775 // 40 true bits as int

class ID
{
private:

    std::bitset<DIGEST> m_id_bits;

    static std::bitset<DIGEST> get_random_id();

public:

    ID()
    {
        m_id_bits = std::bitset<DIGEST>{0};
    };

    static ID create_random()
    {
        ID id;
        id.randomize();
        return id;
    }

    const std::bitset<DIGEST> get_ID_bits()
    {
        return m_id_bits;
    }

    const std::string to_string();

    const std::bitset<DIGEST> distance(const ID & another_id)
    {
        return m_id_bits ^ another_id.m_id_bits;
    }

    void randomize()
    {
        m_id_bits = get_random_id();
    }

    uint16_t prefix_length(const ID & another_ID) const;

    size_t calc_bucket_index(const ID & another_ID) const;
    {
        // calculates bucket index related to current id
    }

    std::array<std::bitset<PART>,BITS> get_partition_id() const;

    friend bool operator==(const ID & l, const ID & r);

//    friend bool operator<(const ID & l, const ID & r)

    // operator std::string() const instead of get_id_string


};
