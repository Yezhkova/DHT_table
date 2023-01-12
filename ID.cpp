
#include <iostream>
#include <bitset>
#include <boost/uuid/detail/sha1.hpp>
#include <boost/algorithm/hex.hpp>
//#include <boost/lexical_cast.hpp>
class ID
{
public:
    std::string m_id;

public:
    ID()
    {
        //randomizer
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
        m_id = buf;
    };

    ID(long long l)
    {
//        ID(boost::lexical_cast<std::string>(l));
        ID(std::to_string(l));
    }

    const std::bitset<160> get_bits_ID() const
    {
        std::bitset<160> m_id_bits;
        for(size_t i = 0; i <= 30; i += 10)
        {
            // 0  30
            // 10 20
            // 20 10
            // 30 0

            std::string s (std::begin(m_id) + i, std::end(m_id) - (m_id.size() - 10 - i));
            std::stringstream ss;
            ss << std::hex << s;
            unsigned long long n;
            ss >> n;
            std::bitset<40> tmp_bitset(n);
            //0 40  80  120
            for(size_t j = 0; j < 40; j++)
            {
                m_id_bits[i * 4 + j] = tmp_bitset[j];
            }
        }
        return m_id_bits;
    }

    const std::string getID()
    {
        return m_id;
    }

    void print()
    {
        std::cout << "[ " << m_id << " ]" << std::endl;
    }

    auto compare(const ID &another_id)
    {
        return get_bits_ID() ^ another_id.get_bits_ID();
    }

private:
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
