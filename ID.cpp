
#include <functional>
#include <iostream>
#include <sstream>
#include <bitset>
#include <boost/uuid/detail/sha1.hpp>
#include <random>
//#include <boost/lexical_cast.hpp>
class ID
{
public:
    std::string m_id_string;
    std::bitset<160> m_id_bits;
public:
    ID()
    {
        auto gen = std::bind( std::uniform_int_distribution<>(0,1),
                              std::default_random_engine()
                            );
        for(size_t i = 0; i < 40; i++)
        {
            m_id_bits[i] = gen();
        }
        m_id_string = get_string_from_bitset();
    };

    ID(const std::string &s)
    {
        boost::uuids::detail::sha1 sha;
        sha.process_bytes(s.data(), s.size());
        unsigned int hash[5] = {0};
        sha.get_digest(hash);
        char buf[41] = {0};
        for (uint16_t i = 0; i < 5; ++i)
        {
            std::sprintf(buf + (i << 3), "%08x", hash[i]);
        }
        m_id_string = buf;
        m_id_bits = get_bitset_from_string();
    };

    ID(long long l)
    {
//        ID(boost::lexical_cast<std::string>(l));
        ID(std::to_string(l));
    }

    const std::string get_ID_string()
    {
        return m_id_string;
    }

    const std::bitset<160> get_ID_bits()
    {
        return m_id_bits;
    }

    void print()
    {
        std::cout << "[ " << m_id_string << " ]" << std::endl;
    }

    auto compare(const ID &another_id)
    {
        return m_id_bits ^ another_id.m_id_bits;
    }

public:

    const std::bitset<160> get_bitset_from_string() const
    {
        std::bitset<160> m_id_string_bits;
        for(uint16_t i = 0; i <= 30; i += 10)
        {
            // 0  30
            // 10 20
            // 20 10
            // 30 0

            std::string s (std::begin(m_id_string) + i, std::end(m_id_string) - (m_id_string.size() - 10 - i));
            std::stringstream ss;
            ss << std::hex << s;
            unsigned long long n;
            ss >> n;
            std::bitset<40> tmp_bitset(n);
            //0 40  80  120
            for(size_t j = 0; j < 40; j++)
            {
                m_id_string_bits[i * 4 + j] = tmp_bitset[j];
            }
        }
        return m_id_string_bits;
    }

    const std::string get_string_from_bitset() const
    {
        std::string m_id_bits_string = "";
        for(uint16_t i = 0; i < 4; i++)
        {
            // 0 39
            // 40 79
            // 80 119
            // 120 159

            std::bitset<40> b;
            for(uint16_t j = 0; j < 40; j++)
            {
                b[j] = m_id_bits[i * 40 + j];
            }
            std::stringstream ss;
            ss << std::hex << b.to_ulong();
            m_id_bits_string += ss.str();
        }
        return m_id_bits_string;
    }

protected:

};

/*
 * bit / 8 chooses an element from your array.
 * Each element contains 8 bits, so dividing by 8
 * is an easy way to convert a bit index to an element index.
 *
 * bit % 8 chooses a bit inside the element.
 * This is most straightforward choice of indexing;
 * it counts bits from the least significant bit to most
 * significant bit (little-endian).
 *
 * Another variant is 7 - bit % 8
 * This variant counts the bits in reverse order (big-endian).
 * Sometimes you have to use it (e.g. in JPEG) for compatibility reasons.
 *
 * (... >> ...) & 1 extracts one bit from a number.
 */
