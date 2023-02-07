
#include "ID.h"
#include "Utils.h"

std::mt19937 ID::random_generator_;

ID::ID() : m_id({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}) {}

const std::array<uint8_t, DIGEST_BYTES>& ID::id() const {
    return m_id;
}

ID ID::randomize()
{
    std::uniform_int_distribution<> range(0, UINT8_MAX);
    for(auto it = m_id.begin(); it != m_id.end(); ++it) {
        *it = range(random_generator_);
    }
    return *this;
}

const std::array<uint16_t, DIGEST_BYTES> ID::distance(const ID & anotherId)
{
    std::array<uint16_t, DIGEST_BYTES> res;
    for(uint16_t i  = 0; i < DIGEST_BYTES; ++i) { // TODO: auto&
        res[i] = m_id[i] ^ anotherId.m_id[i];
    }
    return res;
}

uint16_t ID::prefixLength(const ID & anotherId) const
{
    uint16_t len = 0;
    uint16_t j = 0;
    for(; m_id[j] == anotherId.m_id[j] && j < DIGEST_BYTES; ++j)
    {
        len += 8;
    }
    if(j < DIGEST_BYTES) {
        for(int i = 7; !(((m_id[j] >> i)&1) ^ ((anotherId.m_id[j] >> i)&1)) && i >= 0; --i)
        {
            ++len;
        }
    }
    return len;
}

bool operator <  (const ID & l, const ID & r) {
    return l.m_id < r.m_id;
}

bool operator ==  (const ID & l, const ID & r) {
    return l.m_id == r.m_id;
}

bool operator !=  (const ID & l, const ID & r) {
    return !(l == r);
}

std::ostream& operator<<(std::ostream& out, const ID& id) {
    for(auto it = id.m_id.begin(); it != id.m_id.end(); ++it) {
        out <<  uint16_t(*it);
    }
    return out;
}

ID createRandomId() {
    return ID().randomize();
}

/*
ID::ID()
{
    m_idBits = std::bitset<DIGEST>{0};
    LOG(m_idBits);
};

ID ID::createRandomId()
{
    ID id;
    id.randomize();
    return id;
}

const std::bitset<DIGEST> ID::getIdBits() const
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
    return !(l.m_idBits == r.m_idBits);
}

bool operator<(const ID & l, const ID & r)
{
    return l.m_idBits.to_string() < r.m_idBits.to_string();
}

bool operator<=(const ID & l, const ID & r)
{
    return l.m_idBits.to_string() <= r.m_idBits.to_string();
}

bool operator>(const ID & l, const ID & r)
{
    return r.m_idBits.to_string() < l.m_idBits.to_string();
}

bool operator>=(const ID & l, const ID & r)
{
    return r.m_idBits.to_string() <= l.m_idBits.to_string();
}

std::ostream& operator<<(std::ostream& out, const ID& id)
{
    out << std::string(id);
    return out;
}
*/


