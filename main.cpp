#include "ID.cpp"

int main(void) {

    ID er("elden ring");
    ID ds("dark souls");

    std::bitset<160> c = er.compare(ds);
    std::cout << c;

    ID id(7364);

    id.print();
    auto bits = id.get_bits_ID();
    std::cout << bits;

//    for(int i = 0; i < 160; i++){
//        std::cout << bits[i];
//    }
    std::cout << std::endl;
    return 0;
}



