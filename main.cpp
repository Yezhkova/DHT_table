#include "ID.h"
#include "utils.h"

int main(void) {
    ID noname;
    ID noname2;
    noname2.randomize();
    LOG(noname2.get_ID_bits());
    LOG(noname2.prefix_length(noname));

    return 0;
}



