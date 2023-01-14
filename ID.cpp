
#include <functional>
#include <iostream>
#include <sstream>
#include <bitset>
#include <boost/uuid/detail/sha1.hpp>
#include <random>
//#include <boost/lexical_cast.hpp>

#define DIGEST 160

class ID
{
public:
    std::bitset<DIGEST> m_id_bits;

public:

    ID() // zeros, for random - make static
    {
        m_id_bits = std::bitset<DIGEST>{0};
    };

    const std::bitset<DIGEST> get_ID_bits()
    {
        return m_id_bits;
    }

    const std::string get_ID_string()
    {
        char c [40];
        for(uint16_t i = 0; i < 160; i += 4)
        {
            uint16_t symbol = m_id_bits[i+3] * 8 +
                              m_id_bits[i+2] * 4 +
                              m_id_bits[i+1] * 2 +
                              m_id_bits[i+0] * 1 ;
            std::ostringstream ss;
            ss << std::hex << symbol;
            std::string result = ss.str();
            c[39 - i / 4] = ss.str()[0];
        }
        return std::string(c);
    }

    void print()
    {
        std::cout << std::endl << get_ID_string() << std::endl;
    }

    std::bitset<DIGEST> compare(const ID & another_id)
    {
        return m_id_bits ^ another_id.m_id_bits;
    }

};
