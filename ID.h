#pragma once
#include <functional>
#include <iostream>
#include <sstream>
#include <bitset>
#include <random>

#define DIGEST 160
#define DIGEST_BYTES 20
#define BITS 4 // 4 bits in hexadecimal character
#define PART 40

class ID
{
    static std::mt19937 g_random_generator;

private:
    std::array<uint8_t, DIGEST_BYTES>   m_id;

public:
    ID();
    const std::array<uint8_t, DIGEST_BYTES>& id() const;
    ID randomize();
    const std::array<uint16_t, DIGEST_BYTES> distance(const ID & anotherId);
    uint16_t prefixLength(const ID & anotherId) const;
    friend bool operator <  (const ID & l, const ID & r);
    friend bool operator ==  (const ID & l, const ID & r);
    friend bool operator !=  (const ID & l, const ID & r);
    friend std::ostream& operator<<(std::ostream& out, const ID& id);
};

ID createRandomId();
