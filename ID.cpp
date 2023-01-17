
#include "ID.h"
#include "utils.h"

static std::mt19937 random_generator_;

const std::string ID::to_string()
{
    char c [PART];
    for(uint16_t i = 0; i < DIGEST; i += BITS)
    {
        uint16_t symbol = 0;
        for(int j = BITS - 1; j >= 0; --j)
        {
            symbol += m_id_bits.test(i+j) * utils::pow_2(j);
        }
        c[(PART - 1) - i/BITS] = utils::hex(symbol);
    }
    return std::string(c);
}

std::bitset<DIGEST> ID::get_random_id()
{
    std::uniform_int_distribution<unsigned long long> range(0, 1);
    std::bitset<DIGEST> res;
    for(uint16_t i = 0; i < DIGEST; i++)
    {
        res[i] = range(random_generator_);
    }
    return res;
}

uint16_t ID::prefix_length(const ID & another_ID) const
{
    if(m_id_bits == another_ID.m_id_bits)
    {
        return DIGEST;
    }
    uint16_t len = 0;
    for(; m_id_bits[DIGEST-1-len] == another_ID.m_id_bits[DIGEST-1-len]; len++);
    return len;
}

std::array<std::bitset<PART>,BITS> ID::get_partition_id() const
{
    std::array<std::bitset<PART>,BITS> arr;
    for(uint16_t i = 0; i < BITS; i++)
    {
        std::bitset<PART> tmp;
        for(uint16_t j = 0; j < PART; j++)
        {
            tmp[j] = m_id_bits[i*PART + j];
        }
        arr[i] = tmp;
    }
    return arr;
}

bool operator==(const ID & l, const ID & r)
{
    return l.m_id_bits == r.m_id_bits;
}

