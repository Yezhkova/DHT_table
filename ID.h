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

    std::bitset<DIGEST> m_idBits;

public:

    ID();

    const std::bitset<DIGEST> getIdBits();
    void randomize();

    static ID createRandomId();
    const std::bitset<DIGEST> distance(const ID & anotherId);
    uint16_t prefixLength(const ID & anotherId) const;
    std::array<std::bitset<PART>,BITS> getPartitionId() const;

    operator std::string() const;
    friend bool operator==(const ID & l, const ID & r);
    friend bool operator!=(const ID & l, const ID & r);
    friend bool operator<(const ID & l, const ID & r);

};
