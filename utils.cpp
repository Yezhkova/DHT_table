#include "utils.h"

namespace utils
{

size_t pow_2(size_t power)
{
    size_t res = 1;
    for(size_t i = 0; i < power; i++)
    {
        res *= 2;
    }
    return res;
}

char hex(uint16_t symbol)
{
    std::map<uint16_t, char> bin_to_hex{
        {0, '0'},
        {1, '1'},
        {2, '2'},
        {3, '3'},
        {4, '4'},
        {5, '5'},
        {6, '6'},
        {7, '7'},
        {8, '8'},
        {9, '9'},
        {10, 'a'},
        {11, 'b'},
        {12, 'c'},
        {13, 'd'},
        {14, 'e'},
        {15, 'f'}
    };

    return bin_to_hex.find(symbol)->second;
}
}