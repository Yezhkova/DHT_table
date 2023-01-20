
#include "ID.h"
#include "utils.h"

static std::mt19937 random_generator_;

ID::ID()
{
    m_id_bits = std::bitset<DIGEST>{0};
};

ID ID::create_random_id()
{
    ID id;
    id.randomize();
    return id;
}

const std::bitset<DIGEST> ID::get_ID_bits()
{
    return m_id_bits;
}

const std::bitset<DIGEST> ID::distance(const ID & another_id)
{
    return m_id_bits ^ another_id.m_id_bits;
}

ID::operator std::string() const
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

void ID::randomize()
{
    std::uniform_int_distribution<unsigned long long> range(0, 1);
    std::bitset<DIGEST> res;
    for(uint16_t i = 0; i < DIGEST; ++i)
    {
        m_id_bits[i] = range(random_generator_);
    }
}

uint16_t ID::prefix_length(const ID & another_ID) const
{
    if(m_id_bits == another_ID.m_id_bits)
    {
        return DIGEST;
    }
    uint16_t len = 0;
    for(; m_id_bits[DIGEST-1-len] == another_ID.m_id_bits[DIGEST-1-len]; ++len);
    return len;
}

std::array<std::bitset<PART>,BITS> ID::get_partition_id() const
{
    std::array<std::bitset<PART>,BITS> arr;
    for(uint16_t i = 0; i < BITS; ++i)
    {
        std::bitset<PART> tmp;
        for(uint16_t j = 0; j < PART; ++j)
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

bool operator<(const ID & l, const ID & r)
{
    return l.m_id_bits.to_string() < r.m_id_bits.to_string();
}


