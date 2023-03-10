
#include "ID.h"
#include "Utils.h"

std::mt19937 ID::g_random_generator;

ID::ID() : m_id({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}) {}

const std::array<uint8_t, DIGEST_BYTES>& ID::id() const {
    return m_id;
}

ID ID::uniformRandomize()
{
    std::uniform_int_distribution<> range(0, UINT8_MAX);
    for(auto it = m_id.begin(); it != m_id.end(); ++it) {
        *it = range(g_random_generator);
    }
    return *this;
}

ID ID::normalRandomize()
{
    std::normal_distribution<> range{128, 127};
    for(auto it = m_id.begin(); it != m_id.end(); ++it) {
        *it = std::round(range(g_random_generator));
    }
    return *this;
}

int ID::equalPrefixLength(const ID & anotherId) const
{
    int len = 0;
    int j = 0;
    for(; j < DIGEST_BYTES && m_id[j] == anotherId.m_id[j]; ++j)
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
        out << std::hex << int(*it);
    }
    return out;
}

// MOD
ID createRandomId() {
    return ID().uniformRandomize();
}
