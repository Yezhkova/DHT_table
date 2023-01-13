#include "ID.cpp"

int main(void) {

    ID er("elden ring");
    std::cout << "[ " << er.get_string_from_bitset() << " ]\n";
    er.print();
    ID ds("dark souls");
    std::cout << ds.get_ID_bits() << '\n';
    std::cout << ds.get_bitset_from_string() << '\n';


    std::bitset<160> c = er.compare(ds);

    ID id(7364);

    ID noname1;
    std::cout << "[ " << noname1.get_string_from_bitset() << " ]\n";
    noname1.print();

    ID noname2;
    std::cout << noname2.get_ID_bits() << '\n';
    std::cout << noname2.get_bitset_from_string();


    std::cout << std::endl;
    return 0;
}



