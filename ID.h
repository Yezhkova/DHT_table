#pragma once
#include <functional>
#include <iostream>
#include <sstream>
#include <bitset>
#include <random>

#define DIGEST 160
#define BITS 4 // 4 bits in hexadecimal character
#define PART 40

class ID
{
private:

    std::bitset<DIGEST> m_id_bits;

public:

    ID();

    const std::bitset<DIGEST> get_ID_bits();
    void randomize();

    static ID create_random_id();
    const std::bitset<DIGEST> distance(const ID & another_id);
    uint16_t prefix_length(const ID & another_ID) const;
    std::array<std::bitset<PART>,BITS> get_partition_id() const;

    operator std::string() const;
    friend bool operator==(const ID & l, const ID & r);
    friend bool operator<(const ID & l, const ID & r);

};
