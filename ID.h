#pragma once
#include <functional>
#include <iostream>
#include <sstream>
#include <bitset>
#include <random>
#include <array>

#define DIGEST_BYTES 20
#define BITS 4 // 4 bits in hexadecimal character
#define PART 40

class ID
{
private:
    std::array<uint8_t, DIGEST_BYTES>   m_id;

public:
    ID();
    const std::array<uint8_t, DIGEST_BYTES>& id() const;
    ID uniformRandomize();
    ID normalRandomize();
    int equalPrefixLength(const ID & anotherId) const;
    int distance(const ID & anotherId) const {
        return DIGEST_BYTES * 8 - equalPrefixLength(anotherId);
    }
    friend bool operator <  (const ID & l, const ID & r);
    friend bool operator ==  (const ID & l, const ID & r);
    friend bool operator !=  (const ID & l, const ID & r);
    friend std::ostream& operator<<(std::ostream& out, const ID& id);
};

ID createRandomId();
