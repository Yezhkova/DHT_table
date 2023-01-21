#include "Utils.h"

namespace Utils
{

size_t pow2(size_t power)
{
    return 1 << power;
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
