
#include "ID.h"
#include "Utils.h"

static std::mt19937 random_generator_;

ID::ID()
{
    m_idBits = std::bitset<DIGEST>{0};
};

ID ID::createRandomId()
{
    ID id;
    id.randomize();
    return id;
}

const std::bitset<DIGEST> ID::getIdBits()
{
    return m_idBits;
}

const std::bitset<DIGEST> ID::distance(const ID & another_id)
{
    return m_idBits ^ another_id.m_idBits;
}

ID::operator std::string() const
{
    char c [PART];
    for(uint16_t i = 0; i < DIGEST; i += BITS)
    {
        uint16_t symbol = 0;
        for(int j = BITS - 1; j >= 0; --j)
        {
            symbol += m_idBits.test(i+j) * Utils::pow2(j);
        }
        c[(PART - 1) - i/BITS] = Utils::hex(symbol);
    }
    return std::string(c);
}

void ID::randomize()
{
    std::uniform_int_distribution<unsigned long long> range(0, 1);
    std::bitset<DIGEST> res;
    for(uint16_t i = 0; i < DIGEST; ++i)
    {
        m_idBits[i] = range(random_generator_);
    }
}

uint16_t ID::prefixLength(const ID & another_ID) const
{
    if(m_idBits == another_ID.m_idBits)
    {
        return DIGEST;
    }
    uint16_t len = 0;
    for(; m_idBits[DIGEST-1-len] == another_ID.m_idBits[DIGEST-1-len]; ++len);
    return len;
}

std::array<std::bitset<PART>,BITS> ID::getPartitionId() const
{
    std::array<std::bitset<PART>,BITS> arr;
    for(uint16_t i = 0; i < BITS; ++i)
    {
        std::bitset<PART> tmp;
        for(uint16_t j = 0; j < PART; ++j)
        {
            tmp[j] = m_idBits[i*PART + j];
        }
        arr[i] = tmp;
    }
    return arr;
}

bool operator==(const ID & l, const ID & r)
{
    return l.m_idBits == r.m_idBits;
}

bool operator!=(const ID & l, const ID & r)
{
    return l.m_idBits != r.m_idBits;
}

bool operator<(const ID & l, const ID & r)
{
    return l.m_idBits.to_string() < r.m_idBits.to_string();
}


