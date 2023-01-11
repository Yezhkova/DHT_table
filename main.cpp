#include <boost/chrono.hpp>
#include <iostream>
#include <math.h>

int main(void) {
boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

int a = 1;
int b = 8;
int c = a ^ b;
std::cout << c << std::endl;

boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
std::cout << "took " << sec.count() << " seconds\n";
return 0;
}
