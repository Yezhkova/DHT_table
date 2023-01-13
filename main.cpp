#include "ID.cpp"

int main(void) {

    ID er("elden ring");
    std::cout << "[ " << er.get_string_from_bitset() << " ]\n";
    er.print();
    ID ds("dark souls");

    std::bitset<160> c = er.compare(ds);

    ID id(7364);

    ID noname1;
    ID noname2;



    std::cout << std::endl;
    return 0;
}



